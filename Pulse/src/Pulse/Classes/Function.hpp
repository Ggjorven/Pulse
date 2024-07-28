#pragma once

#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/Traits.hpp"
#include "Pulse/Types/TypeUtils.hpp"

#include <type_traits>

namespace Pulse
{

    // TODO: Properly implement
    
    // Helper to determine if a callable can be constexpr invoked
    template<typename Callable, typename... Args>
    concept IsConstexprInvocable = requires (Callable&& c, Args&&... args)
    {
        { std::invoke(std::forward<Callable>(c), std::forward<Args>(args)...) };
    };

    // The function wrapper
    template<Types::Concepts::Callable Callable>
    class Function
    {
    public:
        using Return = typename Types::FunctionTraits<Callable>::ReturnType;

    public:
        // Constructors
        constexpr Function() = default;
        constexpr Function(Callable func);
        constexpr Function(Function<Callable>&& other) noexcept;
        constexpr ~Function() = default;

        // Operators
        constexpr Function<Callable>& operator = (Callable func);
        constexpr Function<Callable>& operator = (Callable&& func) noexcept;

        template<typename... Args>
        constexpr Return operator()(Args&&... args) const requires IsConstexprInvocable<Callable, Args...>;

    private:
        Callable m_Func;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template<Types::Concepts::Callable Callable>
    constexpr Function<Callable>::Function(Callable func)
        : m_Func(func) {}

    template<Types::Concepts::Callable Callable>
    constexpr Function<Callable>::Function(Function<Callable>&& other) noexcept
        : m_Func(std::move(other.m_Func)) {}

    ///////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////////////////
    template<Types::Concepts::Callable Callable>
    constexpr Function<Callable>& Function<Callable>::operator = (Callable func)
    {
        m_Func = func;
        return *this;
    }

    template<Types::Concepts::Callable Callable>
    constexpr Function<Callable>& Function<Callable>::operator = (Callable&& func) noexcept
    {
        m_Func = std::move(func);
        return *this;
    }

    template<Types::Concepts::Callable Callable>
    template<typename... Args>
    constexpr Function<Callable>::Return Function<Callable>::operator () (Args&&... args) const requires IsConstexprInvocable<Callable, Args...>
    {
        return std::invoke(m_Func, std::forward<Args>(args)...);
    }


}