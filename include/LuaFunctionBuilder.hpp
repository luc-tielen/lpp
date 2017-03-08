#pragma once
#include <string>
#include <memory>  // std::shared_ptr
#include <LuaFunction.hpp>


namespace lpp
{
    class LuaStack;

    template <typename T, typename... Ts>
    class LuaFuncRepresentation
    {
    public:
        LuaFuncRepresentation(const std::shared_ptr<LuaStack>& stack,
                              std::string&& file,
                              std::string&& func_name)
            : m_pstack(stack)
            , m_file(file)
            , m_func_name(func_name) {}

        LuaFunction<T, Ts...> build()
        {
            return LuaFunction<T, Ts...>(m_pstack,
                                         std::move(m_file),
                                         std::move(m_func_name));
        }

    private:
        const std::shared_ptr<LuaStack> m_pstack;
        std::string m_file;
        std::string m_func_name;
    };

    template <typename T>
    class LuaFuncAddParamsStep
    {
    public:
        LuaFuncAddParamsStep(const std::shared_ptr<LuaStack>& stack,
                             std::string&& file,
                             std::string&& func_name)
            : m_pstack(stack)
            , m_file(file)
            , m_func_name(func_name) {}

        template <typename... Ts>
        LuaFuncRepresentation<T, Ts...> with_params()
        {
            return LuaFuncRepresentation<T, Ts...>(m_pstack,
                                                   std::move(m_file),
                                                   std::move(m_func_name));
        }

    private:
        const std::shared_ptr<LuaStack>& m_pstack;
        std::string m_file;
        std::string m_func_name;
    };

    class LuaFuncAddNameStep
    {
    public:
        LuaFuncAddNameStep(const std::shared_ptr<LuaStack>& stack,
                           std::string&& file,
                           std::string&& func_name)
            : m_pstack(stack)
            , m_file(file)
            , m_func_name(func_name) {}

        template <typename T>
        LuaFuncAddParamsStep<T> with_return_type()
        {
            return LuaFuncAddParamsStep<T>(m_pstack,
                                           std::move(m_file),
                                           std::move(m_func_name));
        }

    private:
        const std::shared_ptr<LuaStack>& m_pstack;
        std::string m_file;
        std::string m_func_name;
    };

    /**
     * Step-builder pattern for importing Lua functions into C++.
     */
    class LuaFunctionBuilder
    {
    public:
        LuaFunctionBuilder(const std::shared_ptr<LuaStack>& stack,
                           std::string&& file)
            : m_pstack(stack)
            , m_file(file) {}

        LuaFuncAddNameStep with_name(std::string&& func_name)
        {
            return LuaFuncAddNameStep(m_pstack,
                                      std::move(m_file),
                                      std::move(func_name));
        }
    private:
        const std::shared_ptr<LuaStack>& m_pstack;
        std::string m_file;
    };
}
