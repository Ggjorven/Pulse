#pragma once

#include "../../../vendor/magic_enum/include/magic_enum/magic_enum_all.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace Pulse::Enum
{
	
	///////////////////////////////////////////////////////////
	// Naming
	///////////////////////////////////////////////////////////
	template<auto V>
	inline constexpr auto Name() noexcept -> magic_enum::detail::enable_if_t<decltype(V), std::string_view>
	{
		return magic_enum::enum_name<V>();
	}

	template<typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Name(E value) noexcept -> magic_enum::detail::enable_if_t<E, std::string_view>
	{
		return magic_enum::enum_name<E>(value);
	}

	template<magic_enum::detail::enum_subtype S, typename E>
	[[nodiscard]] inline constexpr auto Name(E value) -> magic_enum::detail::enable_if_t<E, std::string_view>
	{
		return magic_enum::enum_name<S, E>(value);
	}

	template <typename E>
	[[nodiscard]] inline constexpr auto TypeName() noexcept -> magic_enum::detail::enable_if_t<E, std::string_view>
	{
		return magic_enum::enum_type_name<E>();
	}

	///////////////////////////////////////////////////////////
	// Values
	///////////////////////////////////////////////////////////
	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Values() noexcept -> magic_enum::detail::enable_if_t<E, magic_enum::detail::values_t<E, S>>
	{
		return magic_enum::enum_values<E, S>();
	}

	///////////////////////////////////////////////////////////
	// Elements
	///////////////////////////////////////////////////////////
	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto ElementCount() noexcept -> magic_enum::detail::enable_if_t<E, std::size_t>
	{
		return magic_enum::enum_count<E, S>();
	}

	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>, typename F, magic_enum::detail::enable_if_t<E, int> = 0>
	inline constexpr auto ForEach(F&& f)
	{
		return magic_enum::enum_for_each<E, S, F>(f);
	}

	///////////////////////////////////////////////////////////
	// Checks
	///////////////////////////////////////////////////////////
	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Contains(E value) noexcept -> magic_enum::detail::enable_if_t<E, bool>
	{
		return magic_enum::enum_contains<E, S>(value);
	}

	template <magic_enum::detail::enum_subtype S, typename E>
	[[nodiscard]] inline constexpr auto Contains(E value) noexcept -> magic_enum::detail::enable_if_t<E, bool>
	{
		return magic_enum::enum_contains<S, E>(value);
	}

	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>>
	[[nodiscard]] constexpr auto Contains(magic_enum::underlying_type_t<E> value) noexcept -> magic_enum::detail::enable_if_t<E, bool>
	{
		return magic_enum::enum_contains<E, S>(value);
	}

	template <typename E, magic_enum::detail::enum_subtype S = magic_enum::detail::subtype_v<E>, typename BinaryPredicate = std::equal_to<>>
	[[nodiscard]] inline constexpr auto Contains(std::string_view value, BinaryPredicate p = {}) noexcept(magic_enum::detail::is_nothrow_invocable<BinaryPredicate>()) -> magic_enum::detail::enable_if_t<E, bool, BinaryPredicate>
	{
		return magic_enum::enum_contains<E, S>(value);
	}

	///////////////////////////////////////////////////////////
	// Fusing
	///////////////////////////////////////////////////////////
    // Size in bytes (it returns a type with double the size)
    template<size_t Size, typename Enable = void>
    struct FuseType;

    template<> struct FuseType<1> { using Type = uint16_t; };
    template<> struct FuseType<2> { using Type = uint32_t; };
    template<> struct FuseType<4> { using Type = uint64_t; };

	/*
		Example:

		switch (Enum::Fuse(color, direction))
		{
		case Enum::Fuse(Color::RED, Directions::Up): // ...
		case Enum::Fuse(Color::BLUE, Directions::Down): // ...
		// ...
		}
	*/
    template<typename E1, typename E2>
    [[nodiscard]] inline constexpr auto Fuse(E1 e1, E2 e2) -> typename FuseType<sizeof(std::conditional_t<(sizeof(E1) >= sizeof(E2)), std::underlying_type_t<E1>, std::underlying_type_t<E2>>)>::Type
        requires(std::is_enum_v<E1> && std::is_enum_v<E2> && (sizeof(E1) <= sizeof(uint32_t)) && (sizeof(E2) <= sizeof(uint32_t)))
    {
        using Larger = std::conditional_t<(sizeof(E1) >= sizeof(E2)), std::underlying_type_t<E1>, std::underlying_type_t<E2>>;
        using Return = typename FuseType<sizeof(Larger)>::Type;

        return static_cast<Return>((std::underlying_type_t<E1>)e1) << (sizeof(Larger) * 8) | static_cast<Return>((std::underlying_type_t<E2>)e2);
    }

	///////////////////////////////////////////////////////////
	// Bitwise operator namespace
	///////////////////////////////////////////////////////////
	namespace Bitwise
	{

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
		inline constexpr E operator ~ (E rhs) noexcept
		{
			return static_cast<E>(~static_cast<std::underlying_type_t<E>>(rhs));
		}

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
		inline constexpr E operator | (E lhs, E rhs) noexcept
		{
			return static_cast<E>(static_cast<std::underlying_type_t<E>>(lhs) | static_cast<std::underlying_type_t<E>>(rhs));
		}

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
        inline constexpr E operator & (E lhs, E rhs) noexcept
        {
            return static_cast<E>(static_cast<std::underlying_type_t<E>>(lhs) & static_cast<std::underlying_type_t<E>>(rhs));
        }

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
		inline constexpr E operator ^ (E lhs, E rhs) noexcept
		{
			return static_cast<E>(static_cast<std::underlying_type_t<E>>(lhs) ^ static_cast<std::underlying_type_t<E>>(rhs));
		}

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
		inline constexpr E& operator |= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs | rhs);
		}

        template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
        inline constexpr E& operator &= (E& lhs, E rhs) noexcept
        {
            return lhs = (lhs & rhs);
        }

		template <typename E, magic_enum::detail::enable_if_t<E, int> = 0>
		inline constexpr E& operator ^= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs ^ rhs);
		}

	}

	///////////////////////////////////////////////////////////
	// Bitwise operator defines
	///////////////////////////////////////////////////////////
    // From https://github.com/Reputeless/EnumBitmask.git CC0-1.0 Universal LICENSE
    namespace EnumBitmask
    {
        template<typename Enum>
        struct EnumWrapper
        {
        public:
            constexpr explicit operator bool() const noexcept
            {
                return (e != Enum{ 0 });
            }

            constexpr operator Enum() const noexcept
            {
                return e;
            }

        public:
            Enum e;
        };

        template<typename Enum>
        EnumWrapper(Enum e)->EnumWrapper<Enum>;
    }

    #define PULSE_BIT_OPERATORS(e)										\
	static_assert(std::is_enum_v<e>, "e must be an enum type");			\
	[[nodiscard]]														\
	inline constexpr auto operator & (e lhs, e rhs) noexcept			\
	{																	\
		using U = std::underlying_type_t<e>;							\
		return Pulse::Enum::EnumBitmask::EnumWrapper{					\
			e(static_cast<U>(lhs) & static_cast<U>(rhs))};				\
	}																	\
	[[nodiscard]]														\
	inline constexpr auto operator |(e lhs, e rhs) noexcept				\
	{																	\
		using U = std::underlying_type_t<e>;							\
		return Pulse::Enum::EnumBitmask::EnumWrapper{					\
			e(static_cast<U>(lhs) | static_cast<U>(rhs))};				\
	}																	\
	[[nodiscard]]														\
	inline constexpr auto operator ^(e lhs, e rhs) noexcept				\
	{																	\
		using U = std::underlying_type_t<e>;							\
		return Pulse::Enum::EnumBitmask::EnumWrapper{					\
			e(static_cast<U>(lhs) ^ static_cast<U>(rhs)) };				\
	}																	\
	[[nodiscard]]														\
	inline constexpr e operator ~(e value) noexcept						\
	{																	\
		using U = std::underlying_type_t<e>;							\
		return e(~static_cast<U>(value));								\
	}																	\
	inline constexpr e& operator &=(e& lhs, e rhs) noexcept				\
	{																	\
		return lhs = (lhs & rhs);										\
	}																	\
	inline constexpr e& operator |=(e& lhs, e rhs) noexcept				\
	{																	\
		return lhs = (lhs | rhs);										\
	}																	\
	inline constexpr e& operator ^=(e& lhs, e rhs) noexcept				\
	{																	\
		return lhs = (lhs ^ rhs);										\
	}

}
