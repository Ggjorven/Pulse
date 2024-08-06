#pragma once

#include "../vendor/magic_enum/include/magic_enum/magic_enum_all.hpp"

#include <type_traits>
#include <utility>

namespace Pulse::Enum
{

	using namespace magic_enum;

	///////////////////////////////////////////////////////////
	// Naming
	///////////////////////////////////////////////////////////
	template<auto V>
	inline constexpr auto Name() noexcept -> detail::enable_if_t<decltype(V), std::string_view>
	{
		return enum_name<V>();
	}

	template<typename E, detail::enum_subtype S = detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Name(E value) noexcept -> detail::enable_if_t<E, std::string_view>
	{
		return enum_name<E>(value);
	}

	template<detail::enum_subtype S, typename E>
	[[nodiscard]] inline constexpr auto Name(E value) -> detail::enable_if_t<E, std::string_view>
	{
		return enum_name<S, E>(value);
	}

	template <typename E>
	[[nodiscard]] inline constexpr auto TypeName() noexcept -> detail::enable_if_t<E, string_view>
	{
		return enum_type_name<E>();
	}

	///////////////////////////////////////////////////////////
	// Values
	///////////////////////////////////////////////////////////
	template <typename E, detail::enum_subtype S = detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Values() noexcept -> detail::enable_if_t<E, detail::values_t<E, S>>
	{
		return enum_values<E, S>();
	}

	///////////////////////////////////////////////////////////
	// Elements
	///////////////////////////////////////////////////////////
	template <typename E, detail::enum_subtype S = detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto ElementCount() noexcept -> detail::enable_if_t<E, std::size_t>
	{
		return magic_enum::enum_count<E, S>();
	}

	template <typename E, detail::enum_subtype S = detail::subtype_v<E>, typename F, detail::enable_if_t<E, int> = 0>
	inline constexpr auto ForEach(F&& f)
	{
		return enum_for_each<E, S, F>(f);
	}

	///////////////////////////////////////////////////////////
	// Checks
	///////////////////////////////////////////////////////////
	template <typename E, detail::enum_subtype S = detail::subtype_v<E>>
	[[nodiscard]] inline constexpr auto Contains(E value) noexcept -> detail::enable_if_t<E, bool>
	{
		return enum_contains<E, S>(value);
	}

	template <detail::enum_subtype S, typename E>
	[[nodiscard]] inline constexpr auto Contains(E value) noexcept -> detail::enable_if_t<E, bool>
	{
		return enum_contains<S, E>(value);
	}

	template <typename E, detail::enum_subtype S = detail::subtype_v<E>>
	[[nodiscard]] constexpr auto Contains(underlying_type_t<E> value) noexcept -> detail::enable_if_t<E, bool>
	{
		return enum_contains<E, S>(value);
	}

	template <typename E, detail::enum_subtype S = detail::subtype_v<E>, typename BinaryPredicate = std::equal_to<>>
	[[nodiscard]] inline constexpr auto Contains(string_view value, BinaryPredicate p = {}) noexcept(detail::is_nothrow_invocable<BinaryPredicate>()) -> detail::enable_if_t<E, bool, BinaryPredicate>
	{
		return enum_contains<E, S>(value);
	}

	///////////////////////////////////////////////////////////
	// Fusing
	///////////////////////////////////////////////////////////
    // Size in bytes (it returns a type with double the size)
    template<std::size_t Size, typename Enable = void>
    struct FuseType;

    template<>
    struct FuseType<1> { using Type = uint16_t; };
    template<>
    struct FuseType<2> { using Type = uint32_t; };
    template<>
    struct FuseType<4> { using Type = uint64_t; };

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
	// Bitwise operators
	///////////////////////////////////////////////////////////
	namespace Bitwise
	{

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E operator ~ (E rhs) noexcept
		{
			return static_cast<E>(~static_cast<underlying_type_t<E>>(rhs));
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E operator | (E lhs, E rhs) noexcept
		{
			return static_cast<E>(static_cast<underlying_type_t<E>>(lhs) | static_cast<underlying_type_t<E>>(rhs));
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr bool operator & (E lhs, E rhs) noexcept
		{
			return static_cast<underlying_type_t<E>>(lhs) & static_cast<underlying_type_t<E>>(rhs);
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E operator ^ (E lhs, E rhs) noexcept
		{
			return static_cast<E>(static_cast<underlying_type_t<E>>(lhs) ^ static_cast<underlying_type_t<E>>(rhs));
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E& operator |= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs | rhs);
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E& operator ^= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs ^ rhs);
		}

	}

}
