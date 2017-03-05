#include "LuaStack.h"


namespace lpp
{
    LuaStack::LuaStack(lua_State* const plua)
        : m_plua(plua)
    {
        assert(m_plua != nullptr);
        luaL_openlibs(m_plua);  // Load Lua libraries
    }

    LuaStack::~LuaStack()
    {
        lua_close(m_plua);
    }

    Result LuaStack::load_file(const std::string& script_path) const
    {
        return luaL_loadfile(m_plua, script_path.c_str()) == LUA_OK
            ? Result::OK
            : Result::ERROR;
    }

    Result LuaStack::run_file(const std::string& script_path) const
    {
        return luaL_dofile(m_plua, script_path.c_str()) == LUA_OK
            ? Result::OK
            : Result::ERROR;
    }

    Result LuaStack::run_string(const std::string& script_code) const
    {
        return luaL_dostring(m_plua, script_code.c_str()) == LUA_OK
            ? Result::OK
            : Result::ERROR;
    }

    Result LuaStack::pcall(uint32_t param_amount,
                        uint32_t return_amount,
                        int32_t err_handler) const
    {
        // TODO err handling? LuaError class instead of returncode?
        return lua_pcall(m_plua, param_amount, return_amount, err_handler) == LUA_OK
            ? Result::OK
            : Result::ERROR;
    }

    void LuaStack::push(const uint8_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const uint16_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const uint32_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const int8_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const int16_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const int32_t& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const float& value) const
    {
        lua_pushnumber(m_plua, static_cast<const double>(value));
    }
    void LuaStack::push(const double& value) const
    {
        lua_pushnumber(m_plua, value);
    }
    void LuaStack::push(const bool& value) const
    {
        lua_pushboolean(m_plua, value);
    }
    void LuaStack::push(const std::string& value) const
    {
        lua_pushlstring(m_plua, value.c_str(), value.length());
    }

    void LuaStack::pop(uint32_t amount) const
    {
        lua_pop(m_plua, amount);
    }

    void LuaStack::get_global(const std::string& global) const
    {
        lua_getglobal(m_plua, global.c_str());
    }
}

