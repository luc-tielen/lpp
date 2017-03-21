#include <LuaStack.h>


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

    void LuaStack::load_file(const std::string& script_path) const
    {
        if (luaL_loadfile(m_plua, script_path.c_str()) == LUA_OK)
        {
            return;
        }
        auto err_msg = get<std::string>(-1);
        throw LuaError(err_msg);
    }

    void LuaStack::run_file(const std::string& script_path) const
    {
        if (luaL_dofile(m_plua, script_path.c_str()) == LUA_OK)
        {
            return;
        }
        auto err_msg = get<std::string>(-1);
        throw LuaError(err_msg);
    }

    void LuaStack::run_string(const std::string& script_code) const
    {
        if (luaL_dostring(m_plua, script_code.c_str()) == LUA_OK)
        {
            return;
        }
        auto err_msg = get<std::string>(-1);
        throw LuaError(err_msg);
    }

    void LuaStack::pcall(uint32_t param_amount,
                         uint32_t return_amount,
                         int32_t err_handler) const
    {
        if (lua_pcall(m_plua, param_amount, return_amount, err_handler) == LUA_OK)
        {
            return;
        }
        auto err_msg = get<std::string>(-1);
        throw LuaError(err_msg);
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

