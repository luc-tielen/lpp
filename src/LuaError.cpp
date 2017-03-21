#include <LuaError.h>


namespace lpp
{
    const char* LuaError::what() const noexcept
    {
        return runtime_error::what();
    }
}
