#pragma once

#include <ranges>
#include <type_traits>

#include "Pulse/Types/Concepts.hpp"

namespace Pulse::Types
{

    //////////////////////////////////////////////////
    // Types
    //////////////////////////////////////////////////
    template<typename T>
    struct TypeManipulation
    {
    public:
        using CleanType = std::decay_t<T>;
        using AddPointer = std::add_pointer_t<std::remove_reference_t<T>>;
        using AddReference = std::add_lvalue_reference_t<std::remove_reference_t<T>>;

        using RemoveConstAndVolatile = std::remove_cv_t<T>;
    };

    // Removes const and & so const std::string& -> std::string
    template<typename T>
    using Clean = TypeManipulation<T>::CleanType;

    // Adds pointer to T so int -> int*
    template<typename T>
    using AddPointer = TypeManipulation<T>::AddPointer;

    // Adds reference to T so int -> int&
    template<typename T>
    using AddReference = TypeManipulation<T>::AddReference;

    // Removes const/volatile from type so const int -> int && const volatile int -> int
    template<typename T>
    using RemoveConstAndVolatile = TypeManipulation<T>::RemoveConstAndVolatile;



    //////////////////////////////////////////////////
    // Other
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



    template<typename... Types>
    struct TypeGroup {};

}