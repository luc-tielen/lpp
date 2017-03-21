#include <LuaState.h>
#include <LuaFunction.hpp>
#include <catch.hpp>


using lpp::LuaState;
using lpp::LuaFunction;


SCENARIO ("Importing Lua functions into C++")
{
    GIVEN ("An imported Lua function")
    {
        LuaState lua;
        auto add = lua.import_function_from("tests/lua_function_test.lua")
                      .with_name("add")
                      .with_return_type<int32_t>()
                      .with_params<int32_t, int32_t>()
                      .build();
        auto mul_string = lua.import_function_from("tests/lua_function_test.lua")
                             .with_name("multiply_string")
                             .with_return_type<std::string>()
                             .with_params<std::string, uint8_t>()
                             .build();

        WHEN ("the imported function is called")
        {
            auto x1 = add(1, -1);
            auto x2 = add(1, 1);
            auto xxx = mul_string("x", 3);

            THEN ("it will return a result just like in Lua.")
            {
                REQUIRE (x1 == 0);
		        REQUIRE (x2 == 2);
                REQUIRE (xxx == "xxx");
            }
        }
    }

    GIVEN ("A Lua function that raises an error")
    {
        LuaState lua;
        auto bad_func = lua.import_function_from("tests/lua_function_test.lua")
                           .with_name("bad_func")
                           .with_return_type<int32_t>()
                           .with_params<>()
                           .build();

        WHEN ("the imported function is called")
        {
            THEN ("it should raise an exception in C++")
            {
                try
                {
                    bad_func();
                    REQUIRE((false && "code execution should never come here!"));
                }
                catch (lpp::LuaError& e)
                {
                    REQUIRE(std::string(e.what()) == "attempt to call a nil value");
                }
            }
        }
    }
}

