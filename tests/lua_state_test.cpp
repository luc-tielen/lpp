#include <catch.hpp>
#include <LuaState.h>


using lpp::LuaState;
using lpp::Result;


SCENARIO ("LuaState usage (normal usage)")
{
    GIVEN ("A LuaState")
    {
        LuaState lua;

        WHEN ("Lua code is ran from a string in memory")
        {
            auto r = lua.run_string("x = 1 + 4");
            auto s = lua.get_stack();
            s->get_global("x");
            auto x = s->get<uint32_t>(-1);

            THEN("the result of the script should be as expected in C++")
            {
                REQUIRE (r == Result::OK);
                REQUIRE (x == 5);
            }
        }

        AND_WHEN ("a Lua script is ran from a file")
        {
            auto r = lua.run_file("tests/lua_state_test.lua");
            auto s = lua.get_stack();
            s->get_global("x");
            auto x = s->get<uint32_t>(-1);

            THEN("the result of the script should be as expected in C++")
            {
                REQUIRE (r == Result::OK);
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
            Result r = lua.run_file("incorrect.lua");
            auto err = lua.get_stack()->get<std::string>(-1);

            THEN ("an error should be returned.")
            {
                REQUIRE (r == Result::ERROR);
                REQUIRE (err == "cannot open incorrect.lua: No such file or directory");
            }
        }

        AND_WHEN ("attempting to run Lua code with errors")
        {
            Result r = lua.run_string("unknown_func()");
            auto err = lua.get_stack()->get<std::string>(-1);

            THEN ("an error should be returned.")
            {
                REQUIRE (r == Result::ERROR);
                REQUIRE (err == "[string \"unknown_func()\"]:1: attempt to call a nil value (global 'unknown_func')");
            }
        }
    }
}
