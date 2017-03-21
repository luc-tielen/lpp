#include <catch.hpp>
#include <LuaState.h>


using lpp::LuaState;


SCENARIO ("LuaState usage (normal usage)")
{
    GIVEN ("A LuaState")
    {
        LuaState lua;

        WHEN ("Lua code is ran from a string in memory")
        {
            lua.run_string("x = 1 + 4");
            auto s = lua.get_stack();
            s->get_global("x");
            auto x = s->get<uint32_t>(-1);

            THEN("the result of the script should be as expected in C++")
            {
                REQUIRE (x == 5);
            }
        }

        AND_WHEN ("a Lua script is ran from a file")
        {
            lua.run_file("tests/lua_state_test.lua");
            auto s = lua.get_stack();
            s->get_global("x");
            auto x = s->get<uint32_t>(-1);

            THEN("the result of the script should be as expected in C++")
            {
                REQUIRE (x == 0xb7a121);
            }
        }
    }
}

SCENARIO ("LuaState usage (incorrect usage)")
{
    GIVEN ("A LuaState")
    {
        LuaState lua;

        WHEN ("attempting to run non-existing file")
        {
            THEN ("an error should be raised.")
            {
                try
                {
                    lua.run_file("incorrect.lua");
                    REQUIRE ((false && "unreachable code!"));
                }
                catch (lpp::LuaError e)
                {
                    REQUIRE (std::string(e.what) == "cannot open incorrect.lua: No such file or directory");
                }
            }
        }

        AND_WHEN ("attempting to run Lua code with errors")
        {
            THEN ("an error should be returned.")
            {
                try
                {
                    lua.run_string("unknown_func()");
                    REQUIRE ((false && "unreachable code!"));
                }
                catch (lpp::LuaError e)
                {
                    auto expected_error = "[string \"unknown_func()\"]:1: attempt to call a nil value (global 'unknown_func')");
                    REQUIRE (std::string(e.what) == expected_error);

                }
            }
        }
    }
}
