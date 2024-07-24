#pragma once

#include "../vendor/magic_enum/include/magic_enum/magic_enum_all.hpp"


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
	/*
		Example:

		switch (Enum::Fuse(color, direction).value()) 
		{
		case Enum::Fuse(Color::RED, Directions::Up).value(): // ...
		case Enum::Fuse(Color::BLUE, Directions::Down).value(): // ...
		// ...
		}
	*/
	template <typename... Es>
	[[nodiscard]] inline constexpr auto Fuse(Es... values) noexcept 
	{
		return enum_fuse<Es...>(values...);
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
		inline constexpr E operator & (E lhs, E rhs) noexcept
		{
			return static_cast<E>(static_cast<underlying_type_t<E>>(lhs) & static_cast<underlying_type_t<E>>(rhs));
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
		inline constexpr E& operator &= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs & rhs);
		}

		template <typename E, detail::enable_if_t<E, int> = 0>
		inline constexpr E& operator ^= (E& lhs, E rhs) noexcept
		{
			return lhs = (lhs ^ rhs);
		}

	}
	
}