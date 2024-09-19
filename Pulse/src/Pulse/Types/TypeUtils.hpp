#pragma once

#include <ranges>
#include <type_traits>

#include "Pulse/Types/Concepts.hpp"

namespace Pulse::Types
{

    //////////////////////////////////////////////////
    // Other
    //////////////////////////////////////////////////
    template<typename T>
    using Clean = std::decay_t<T>;

    template <typename Base, typename Derived>
    struct SameImpl : std::is_same<Base, Derived> {};

    // Checks if T1 == T2
    template <typename T1, typename T2>
    inline constexpr bool Same = SameImpl<T1, T2>::value;



    template<typename... Types>
    struct TypeGroup {};

}