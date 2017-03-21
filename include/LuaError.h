#pragma once
#include <stdexcept>


namespace lpp
{
    class LuaError : public std::runtime_error
    {
    public:
        LuaError(const std::string& msg)
            : std::runtime_error(msg) {}
        LuaError(const LuaError&) = default;
        LuaError& operator=(const LuaError&) = default;
        LuaError(LuaError&&) noexcept = default;
        LuaError& operator=(LuaError&&) noexcept = default;
        virtual ~LuaError() {}

        virtual const char* what() const noexcept override;
    };
}
