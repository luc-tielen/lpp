#include <LuaState.h>
#include <catch.hpp>
#include <sstream>


using lpp::LuaState;
using lpp::LuaFunction;


int32_t add(int32_t x, int32_t y);
std::string multiply_string(std::string str, uint8_t count);


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


SCENARIO ("Importing C++ functions into Lua")
{
    GIVEN ("An exported C++ function")
    {
        LuaState lua;
        lua.export_function(add, "add");
        lua.export_function(multiply_string, "multiply_string");

        WHEN ("the exported functions are called in Lua")
        {
            lua.run_file("tests/cpp_function_test.lua");

            THEN ("it will return the same results as it would when called from C++ directly.")
            {
                auto s = lua.get_stack();
                s->get_global("x");
                s->get_global("y");
                s->get_global("z");
                auto x = s->get<int32_t>(-3);
                auto y = s->get<int32_t>(-2);
                auto z = s->get<std::string>(-1);

                REQUIRE (x == add(123, 456));
                REQUIRE (y == add(-123, 789));
		        REQUIRE (z == multiply_string("x", 3));
            }
        }
    }
}

//TODO multiple return types... ; lambda / function pointer
//TODO fix types (string cant be passed as const& yet)
