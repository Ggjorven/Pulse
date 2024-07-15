#pragma once

#include <iterator>
#include <type_traits>
#include <concepts>

namespace Pulse::Types::Concepts
{

    // Concept to check if a type T has begin and end methods (aka is iterable)
    template <typename T>
    concept Iterable = requires(T t) {
        { t.begin() } -> std::input_or_output_iterator;
        { t.end() } -> std::input_or_output_iterator;
    };

    // Concept to check if Derived inherits from base
    template<typename Base, typename Derived>
    concept InheritsFrom = std::is_base_of<Base, Derived>::value;

}