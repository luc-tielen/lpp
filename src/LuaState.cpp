#include <assert.h>
#include <stdexcept>
#include <lua.hpp>
#include <LuaStack.h>
#include <LuaState.h>


namespace lpp
{
    LuaState::LuaState()
        : m_pstack(std::make_shared<LuaStack>(luaL_newstate())) {}

    LuaState::~LuaState() {}

    void LuaState::load_file(const std::string& script_path) const
    {
        m_pstack->load_file(script_path);
    }

    void LuaState::run_file(const std::string& script_path) const
    {
        m_pstack->run_file(script_path);
    }

    void LuaState::run_string(const std::string& script_code) const
    {
        m_pstack->run_string(script_code);
    }

    LuaFunctionBuilder LuaState::import_function_from(std::string&& file) const
    {
        return LuaFunctionBuilder(m_pstack, std::move(file));
    }

    const std::shared_ptr<LuaStack>& LuaState::get_stack() const
    {
        return m_pstack;
    }
}
