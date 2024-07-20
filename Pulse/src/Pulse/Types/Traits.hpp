#pragma once

#include <utility>
#include <concepts>
#include <functional>
#include <type_traits>

namespace Pulse::Types
{

    ///////////////////////////////////////////////////////////
    // Function Traits
    ///////////////////////////////////////////////////////////
    template <typename>
    struct FunctionTraitsImpl;

    // Specialization for function types
    template <typename Ret, typename... Args>
    struct FunctionTraitsImpl<Ret(Args...)>
    {
        using ReturnType = Ret;
        using ArgsTuple = std::tuple<Args...>;
    };

    // Specialization for function pointers
    template <typename Ret, typename... Args>
    struct FunctionTraitsImpl<Ret(*)(Args...)> : FunctionTraitsImpl<Ret(Args...)> {};

    // Specialization for std::function
    template <typename Ret, typename... Args>
    struct FunctionTraitsImpl<std::function<Ret(Args...)>> : FunctionTraitsImpl<Ret(Args...)> {};

    // Specialization for member function pointers
    template <typename Ret, typename Class, typename... Args>
    struct FunctionTraitsImpl<Ret(Class::*)(Args...)> : FunctionTraitsImpl<Ret(Args...)> {};
    template <typename Ret, typename Class, typename... Args>
    struct FunctionTraitsImpl<Ret(Class::*)(Args...) const> : FunctionTraitsImpl<Ret(Args...)> {};

    // Specialization for callable objects
    template <typename Callable>
    struct FunctionTraitsImpl : FunctionTraitsImpl<decltype(&Callable::operator())> {};

    template<typename Function>
    struct FunctionTraits
    {
        using ReturnType = FunctionTraitsImpl<Function>::ReturnType;
        using ArgsTuple = FunctionTraitsImpl<Function>::ArgsTuple;
    };

    //////////////////////////////////////////////////
    // Ranges Traits
    //////////////////////////////////////////////////
    template<typename T>
    struct RangeTraitsImpl
    {
        using Iterator = std::ranges::iterator_t<T>;
        using Sentinel = std::ranges::sentinel_t<T>;
    };

    template<typename Range>
    struct RangeTraits
    {
        using Iterator = RangeTraitsImpl<Range>::Iterator;
        using Sentinel = RangeTraitsImpl<Range>::Sentinel; // Sentinel is the end of the iterator
    };

    //////////////////////////////////////////////////
    // Iterator Traits
    //////////////////////////////////////////////////
    template<typename T>
    struct IteratorTraitsImpl
    {
        using ValueType = std::iterator_traits<T>::value_type;
    };

    template<typename Iterator>
    struct IteratorTraits
    {
        using ValueType = IteratorTraitsImpl<Iterator>::ValueType;
    };

}