#include <LuaState.h>
#include <catch.hpp>
#include <sstream>


using lpp::LuaState;
using lpp::LuaFunction;

static const std::string ERROR_MSG = "an example error msg";


int32_t add(int32_t x, int32_t y);
std::string multiply_string(std::string str, uint8_t count);
int bad_function(int);
void void_function();
int identity(int);


int32_t add(int32_t x, int32_t y)
{
    return x + y;
}


std::string multiply_string(std::string str, uint8_t count)
{
    std::ostringstream os;
    for (auto i = 0u; i < count; ++i) { os << str; }
    return os.str();
}


int bad_function(int)
{
    throw std::runtime_error(ERROR_MSG);
}

void void_function() {}

int identity(int x) { return x; }


SCENARIO ("Importing C++ functions into Lua")
{
    GIVEN ("Exported C++ functions to Lua interpreter")
    {
        LuaState lua;
        lua.export_function(add, "add");
        lua.export_function(multiply_string, "multiply_string");
        lua.export_function(void_function, "void_function");
        lua.export_function(bad_function, "bad_function");
        lua.export_function(identity, "identity");

        WHEN ("the exported functions are called in Lua")
        {
            lua.run_file("tests/cpp_function_test.lua");

            THEN ("it will return the same results as it would when called from C++ directly.")
            {
                auto s = lua.get_stack();
                s->get_global("x");
                s->get_global("y");
                s->get_global("z");
                s->get_global("five");
                auto x = s->get<int32_t>(-4);
                auto y = s->get<int32_t>(-3);
                auto z = s->get<std::string>(-2);
                auto five = s->get<int>(-1);

                REQUIRE (x == add(123, 456));
                REQUIRE (y == add(-123, 789));
		        REQUIRE (z == multiply_string("x", 3));
                REQUIRE (five == 5);
            }
        }

        AND_WHEN ("a C++ function raises an error")
        {
            THEN ("it is forwarded to the Lua interpreter to be handled there.")
            {
                // NOTE: in Lua the error is raised again so it is caught back in C++ for testing purposes
                try
                {
                    lua.run_file("tests/bad_cpp_function_test.lua");
                    REQUIRE ((false && "unreachable code!"));
                }
                catch (lpp::LuaError& e)
                {
                    auto err_msg = std::string("tests/bad_cpp_function_test.lua:5: ")
                                 + "Error from C++, rethrown in Lua";
                    REQUIRE (std::string(e.what()) == err_msg);
                }
            }
        }
    }
}

//TODO multiple return types... ; lambda / function pointer
