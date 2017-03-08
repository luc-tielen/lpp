#pragma once
#include <assert.h>
#include <lua.hpp>
#include <string>
#include <LuaStack.h>


namespace lpp
{
    /**
     * Template class for calling a Lua function in C++ as if it were a normal
     * C++ function. Uses variadic templates to achieve type-safeness.
     */
    template <typename T, typename... Ts>
    class LuaFunction
    {
    public:
        LuaFunction(const std::shared_ptr<LuaStack>& stack,
                    std::string&& file,
                    std::string&& func_name)
            : m_pstack(stack)
            , m_file(file)
            , m_func_name(func_name)
        {
            assert(m_pstack);

            //TODO move to load file?
            if (m_pstack->load_file(m_file) == Result::ERROR)
            {
                // TODO create LuaError class..
                const std::string err = std::string("Could not load file: ")
                                      + m_pstack->get<std::string>(-1);
                throw std::runtime_error(err);
            }

            // TODO move to pcall?
            if (m_pstack->pcall(0, 0, 0) == Result::ERROR)  // Prime the file once to load globals
            {
                const std::string err = std::string("Could not prime file: ")
                                      + m_pstack->get<std::string>(-1);
                throw std::runtime_error(err);
            }
        }
        LuaFunction(const LuaFunction&) = default;
        LuaFunction& operator=(const LuaFunction&) = default;
        LuaFunction(LuaFunction&& other)
            : m_pstack(other.m_pstack)
            , m_file(std::move(other.m_file))
            , m_func_name(std::move(other.m_func_name)) {}
        LuaFunction& operator=(LuaFunction&& other)
        {
            m_pstack = other.m_pstack;
            m_file = std::move(other.m_file);
            m_func_name = std::move(other.m_func_name);
            return *this;
        }
        ~LuaFunction() {}

        T operator()(const Ts&... args)
        {
            LuaStack& stack = *m_pstack;
            stack.get_global(m_func_name);       // Push function on stack
            push_on_stack(args...);              // Push values on stack
            stack.pcall(sizeof...(args), 1, 0);  // Execute function   TODO error handling
            T result = stack.get<T>(-1);         // Get result (now on top of stack)
            stack.pop(1);                        // Pop return value of stack (cleanup)
            return result;
        }

    private:
        const std::shared_ptr<LuaStack> m_pstack;
        std::string m_file;
        std::string m_func_name;

        // Helper functions:

        void push_on_stack() {}

        template <typename Arg, typename... Args>
        void push_on_stack(const Arg& arg, const Args&... args)
        {
            m_pstack->push(arg);
            push_on_stack(args...);
        }
    };
}
