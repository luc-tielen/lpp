#pragma once
#include <lua.hpp>


namespace lpp
{
    template <typename ReturnType, typename... ParameterTypes>
    using ExportableFunction = ReturnType (*)(ParameterTypes...);
    // TODO using Lambda typedef?


    /**
     * Helper class for getting an element of the Lua stack.
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
            if (!str) { return ""; }
            return str;
        }

    private:
        lua_State* m_plua;
        const int& m_location;
    };


    /**
     * Helper function for pushing an element onto the Lua stack.
     */
    inline void push_on_stack(lua_State* plua, const uint8_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const uint16_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const uint32_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const int8_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const int16_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const int32_t& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const float& value)
    {
        assert(plua);
        lua_pushnumber(plua, static_cast<const double>(value));
    }
    inline void push_on_stack(lua_State* plua, const double& value)
    {
        assert(plua);
        lua_pushnumber(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const bool& value)
    {
        assert(plua);
        lua_pushboolean(plua, value);
    }
    inline void push_on_stack(lua_State* plua, const std::string& value)
    {
        assert(plua);
        lua_pushlstring(plua, value.c_str(), value.length());
    }


    template <typename ParamType, typename... AccumParamTypes>
    auto do_fetch_param(lua_State* plua_state, int index_of_param,
                        std::tuple<AccumParamTypes...>&& params)
    {
        ParamType value = LuaStackGetter(plua_state, index_of_param + 1);
        return std::tuple_cat(std::forward<std::tuple<AccumParamTypes...>>(params),
                              std::make_tuple(std::forward<ParamType>(value)));
    }

    template <typename...>
    struct do_fetch_params {};

    template <typename LastParamType,
              typename... AccumParamTypes>
    struct do_fetch_params<std::tuple<LastParamType>,
                           std::tuple<AccumParamTypes...>>
    {
        auto operator()(lua_State* plua_state, int i,
                        std::tuple<AccumParamTypes...>&& params)
        {
            using ParamTypes = std::tuple<AccumParamTypes...>;
            return do_fetch_param<LastParamType>(plua_state, i,
                                                 std::forward<ParamTypes>(params));
        }
    };

    template <typename FirstParamType,
              typename... RestParamTypes,
              typename... AccumParamTypes>
    struct do_fetch_params<std::tuple<FirstParamType, RestParamTypes...>,
                           std::tuple<AccumParamTypes...>>
    {
        auto operator()(lua_State* plua_state, int i,
                        std::tuple<AccumParamTypes...>&& params)
        {
            using ParamTypes = std::tuple<AccumParamTypes...>;
            auto params_updated = do_fetch_param<FirstParamType>(
                plua_state, i,
                std::forward<ParamTypes>(params));
            return do_fetch_params<std::tuple<RestParamTypes...>,
                                   decltype(params_updated)>{}(
                                       plua_state, i + 1,
                                       std::forward<decltype(params_updated)>(params_updated));
        }
    };

    template <typename... ParamTypes>
    auto fetch_params(lua_State *plua_state)
    {
        return do_fetch_params<std::tuple<ParamTypes...>,
                               std::tuple<>>{}(plua_state, 0, std::make_tuple());
    }

    template <typename ReturnType, typename... ParamTypes, size_t... Is>
    auto apply_function(ExportableFunction<ReturnType, ParamTypes...> f,
                        std::tuple<ParamTypes...>&& params,
                        std::index_sequence<Is...>)
    {
        return f(std::get<Is>(params)...);
    }

    template <typename ReturnType, typename... ParamTypes>
    int do_call(lua_State* plua_state)
    {
        using Function = ExportableFunction<ReturnType, ParamTypes...>;
        constexpr size_t num_args = sizeof...(ParamTypes);
        constexpr auto indices = std::make_index_sequence<num_args>{};

        auto f = *reinterpret_cast<Function*>(lua_touserdata(plua_state, lua_upvalueindex(1)));
        auto params = fetch_params<ParamTypes...>(plua_state);
        auto result = apply_function(f, std::forward<decltype(params)>(params), indices);
        lua_pop(plua_state, num_args);
        push_on_stack(plua_state, result);
        return 1;
    }

    // Helper function to export C++ functions to Lua.
    template <typename ReturnType, typename... ParamTypes>
    void export_function_helper(lua_State* plua_state,
                                ExportableFunction<ReturnType, ParamTypes...> f,
                                std::string&& lua_function_name)
    {
        assert(plua_state && "Lua state not allowed to be nullptr!");
        assert(f && "Function not allowed to be nullptr!");
        lua_pushlightuserdata(plua_state, &f);
        lua_pushcclosure(plua_state, &do_call<ReturnType, ParamTypes...>, 1);
        lua_setglobal(plua_state, lua_function_name.c_str());
    }
}
