#pragma once
#include <assert.h>
#include <cstdint>
#include <string>
#include <lua.hpp>
#include <Result.h>
#include <LuaStackHelpers.hpp>


namespace lpp
{
    /**
     * C++ wrapper around the Lua virtual stack.
     * Provides both high- and low-level operations.
     */
    class LuaStack
    {
    public:
        LuaStack(lua_State* const pLua);
        LuaStack(const LuaStack& other) = delete;
        LuaStack& operator=(const LuaStack& other) = delete;
        LuaStack(LuaStack&& other) = default;
        LuaStack& operator=(LuaStack&& other) = default;
        ~LuaStack();

        // High level operations:

        /**
         * Loads a Lua script.
         */
        Result load_file(const std::string& script_path) const;

        /**
         * Loads a Lua script from a file and runs it.
         */
        Result run_file(const std::string& script_path) const;

        /**
         * Runs a Lua script from a string in memory.
         */
        Result run_string(const std::string& script_code) const;

        /**
         * Do a protected call of a function with X amount of params and Y
         * return values and err handler on location 'err_handler_loc'.
         */
        Result pcall(uint32_t param_amount,
                     uint32_t return_amount,
                     int32_t err_handler_loc) const;

        // Low level operations:

        /**
         * Gets an element of the stack at a certain position.
         * if location >= 0: get position at location starting from
         *                   beginning of stack
         * if location < 0: get position, location starting from top of stack
         *                  (-1 = top of stack)
         */
        template <typename T>
        T get(const int& location) const
        {
            return LuaStackGetter(m_plua, location);
        }

        template <typename T>
        void push(const T& value) const
        {
            StackPusher(m_plua).push(value);
        }

        /**
         * Pops X amount of elements of the stack.
         */
        void pop(uint32_t amount) const;

        /**
         * Gets a global from Lua and puts it on top of the stack.
         */
        void get_global(const std::string& global) const;

        /**
         * Exports a function from C++ to Lua.
         */
        template <typename ReturnType, typename... ParameterTypes>
        Result export_function(ExportableFunction<ReturnType, ParameterTypes...> f,
                               std::string&& lua_function_name) const
        {
            return export_function_helper(m_plua, f, std::move(lua_function_name));
        }

    private:
        lua_State* const m_plua;
    };
}
