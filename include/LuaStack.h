#pragma once
#include <assert.h>
#include <cstdint>
#include <string>
#include <lua.hpp>
#include <Result.h>


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

        /**
         * Pushes an element onto the stack.
         */
        void push(const uint8_t& value) const;
        void push(const uint16_t& value) const;
        void push(const uint32_t& value) const;
        //void push(const uint64_t& value)
        void push(const int8_t& value) const;
        void push(const int16_t& value) const;
        void push(const int32_t& value) const;
        //void push(const int64_t& value)
        void push(const float& value) const;
        void push(const double& value) const;
        void push(const bool& value) const;
        void push(const std::string& value) const;

        /**
         * Pops X amount of elements of the stack.
         */
        void pop(uint32_t amount) const;

        /**
         * Gets a global from Lua and puts it on top of the stack.
         */
        void get_global(const std::string& global) const;

    private:
        lua_State* const m_plua;

        /**
         * Helper class for getting an element of the stack.
         */
        class LuaStackGetter
        {
        public:
            LuaStackGetter(lua_State* plua, const int& location)
                : m_plua(plua)
                , m_location(location)
            {
                assert(plua);
            }

            operator uint8_t() const
            {
                return static_cast<uint8_t>(lua_tonumber(m_plua, m_location));
            }
            operator uint16_t() const
            {
                return static_cast<uint16_t>(lua_tonumber(m_plua, m_location));
            }
            operator uint32_t() const
            {
                return static_cast<uint32_t>(lua_tonumber(m_plua, m_location));
            }
            operator int8_t() const
            {
                return static_cast<int8_t>(lua_tonumber(m_plua, m_location));
            }
            operator int16_t() const
            {
                return static_cast<int16_t>(lua_tonumber(m_plua, m_location));
            }
            operator int32_t() const
            {
                return static_cast<int32_t>(lua_tonumber(m_plua, m_location));
            }
            operator float() const
            {
                return static_cast<float>(lua_tonumber(m_plua, m_location));
            }
            operator double() const
            {
                return static_cast<double>(lua_tonumber(m_plua, m_location));
            }
            operator bool() const
            {
                return lua_toboolean(m_plua, m_location);
            }
            operator std::string() const
            {
                const char* str = lua_tostring(m_plua, m_location);
                if (!str) str = "";
                return str;
            }

        private:
            lua_State* m_plua;
            const int& m_location;
        };
    };
}
