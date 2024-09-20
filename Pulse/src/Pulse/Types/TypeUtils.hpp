#pragma once

#include <ranges>
#include <type_traits>

#include "Pulse/Types/Concepts.hpp"

namespace Pulse::Types
{

    //////////////////////////////////////////////////
    // Utils
    //////////////////////////////////////////////////
    template<typename T>
    using Clean = std::decay_t<T>;

    //////////////////////////////////////////////////
    // Main
    //////////////////////////////////////////////////
    template <typename Base, typename Derived>
    struct InheritsFromImpl : std::is_base_of<Base, Derived> {};

    // Checks if Derived inherits from Base
    template <typename Base, typename Derived>
    inline constexpr bool InheritsFrom = InheritsFromImpl<Base, Derived>::value;

    template <typename Base, typename Derived>
    struct SameImpl : std::is_same<Base, Derived> {};

    // Checks if T1 == T2
    template <typename T1, typename T2>
    inline constexpr bool Same = SameImpl<T1, T2>::value;

    //////////////////////////////////////////////////
    // Other
    //////////////////////////////////////////////////
    template<typename... Types>
    struct TypeGroup {};

}