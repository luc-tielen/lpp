#pragma once
#include <string>
#include <LuaFunctionBuilder.hpp>
#include <Result.h>


namespace lpp
{
    class LuaStack;

    /**
     * Class responsible for managing a Lua instance.
     * Provides high level functions for interfacing with the Lua side of things.
     */
    class LuaState
    {
    public:
        /**
         * Initializes the LuaState.
         */
        LuaState();
        LuaState(const LuaState& other) = delete;
        LuaState& operator=(const LuaState& other) = delete;
        LuaState(LuaState&& other) = default;
        LuaState& operator=(LuaState&& other) = default;

        /**
         * Destructor for the LuaState.
         */
        ~LuaState();

        /**
         * Loads a Lua script.
         */
        Result load_file(const std::string& script_path) const;

        /**
         * Loads a Lua script from a file and runs it.
         */
        Result run_file(const std::string& script_path) const;

        /**
         * Runs a script from a Lua string in memory.
         */
        Result run_string(const std::string& script_code) const;

        /**
         * Helper function for importing a Lua function into C++.
         * Returns a builder object which can create a Lua function with a
         * certain amount of params and return values.
         */
        LuaFunctionBuilder import_function_from(std::string&& file) const;

        /**
         * Get the interface to the lower level Lua stack.
         */
        const std::shared_ptr<LuaStack>& get_stack() const;

    private:
        std::shared_ptr<LuaStack> m_pstack;
    };
}
