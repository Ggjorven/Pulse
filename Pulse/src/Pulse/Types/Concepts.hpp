#pragma once

#include <ranges>
#include <utility>
#include <iterator>
#include <concepts>
#include <functional>
#include <type_traits>

#include "Pulse/Types/Traits.hpp"

namespace Pulse::Types::Concepts
{

    // Concept to check if a type T has begin and end methods (aka is iterable)
    template <typename T>
    concept Iterable = requires(T t) 
    {
        { t.begin() } -> std::input_or_output_iterator;
        { t.end() } -> std::input_or_output_iterator;
    };

    // Define a concept to check if a type T is an iterator
    template <typename T>
    concept Iterator = requires(T t) 
    {
        { *t } -> std::convertible_to<typename std::iterator_traits<T>::value_type>;

        { ++t } -> std::same_as<T&>;
        { t++ } -> std::same_as<T>;

        { t == t } -> std::convertible_to<bool>;
    };

    // Concept to check if a type is a range
    template <typename T>
    concept Range = requires(T t)
    {
        std::ranges::begin(t);
        std::ranges::end(t);
    };

    // Concept to check if a type is a function
    template <typename F>
    concept Callable = requires (F f, FunctionTraits<F>::ArgsTuple args) 
    {
        std::apply(f, args);
    };

    // Concept to check if a type T is callable with specific argument types and returns a specific type
    template <typename Function, typename ReturnType, typename... Args>
    concept CallableWithArgs = requires(Function callable, Args... args)
    {
        { callable(args...) } -> std::same_as<ReturnType>;
    };

    // Concept to check if Derived inherits from base
    template<typename Base, typename Derived>
    concept InheritsFrom = std::is_base_of<Base, Derived>::value;

}