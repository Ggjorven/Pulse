#pragma once

#include <type_traits>

namespace Pulse::Types
{

    template<typename T>
    struct CleanImpl
    {
        using Type = std::decay_t<T>;
    };

    // Removes const and & so const std::string& -> std::string
    template<typename T>
    using Clean = CleanImpl<T>::Type;

    template<typename T>
    struct AddPointerImpl 
    {
        using Type = T*;
    };

    // Adds pointer to T so int -> int*
    template<typename T>
    using AddPointer = AddPointerImpl<T>::Type;

    template<typename T>
    struct AddReferenceImpl
    {
        using Type = T&;
    };

    // Adds reference to T so int -> int&
    template<typename T>
    using AddReference = AddReferenceImpl<T>::Type;

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



    template<typename... Types>
    struct TypeGroup {};
}