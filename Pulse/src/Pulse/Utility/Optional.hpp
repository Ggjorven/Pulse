#pragma once

#include "Pulse/Core/Core.hpp"
#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Types/Concepts.hpp"

#include <optional>

namespace Pulse::Utility
{

	template<typename T>
	concept OptionalT = Types::Concepts::CopyConstructible<T> && Types::Concepts::MoveConstructible<T> && Types::Concepts::Destructible<T>;

	template<OptionalT T>
	class Optional 
	{
	public:
		constexpr static const bool ValueChecking = true;
	public:
		// Constructors
		Optional() = default;
		Optional(const T& value);
		~Optional() = default;

		// Operators
		operator bool();

		operator T& ();
		operator const T&() const;

		Optional<T> operator = (const T& value);

		// Methods
		inline bool HasValue() const { return m_HasValue; }

		T& Value();
		const T& Value() const;

	private:
		bool m_HasValue = false;
		T m_Value = {};
	};

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	template<OptionalT T>
	Optional<T>::Optional(const T& value)
		: m_Value(value), m_HasValue(true) {}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	template<OptionalT T>
	Optional<T>::operator bool()
	{
		return HasValue();
	}

	template<OptionalT T>
	Optional<T>::operator T&()
	{
		return Value();
	}

	template<OptionalT T>
	Optional<T>::operator const T&() const
	{
		return Value();
	}

	template<OptionalT T>
	Optional<T> Optional<T>::operator = (const T& value)
	{
		return Optional<T>(value);
	}

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	template<OptionalT T>
	T& Optional<T>::Value()
	{
		if constexpr (ValueChecking)
		{
			if (!m_HasValue)
				Logger::Log(LogLevel::Fatal, "Accessing Optional<T> value with no value.");
		}

		return m_Value;
	}

	template<OptionalT T>
	const T& Optional<T>::Value() const
	{
		if constexpr (ValueChecking)
		{
			if (!m_HasValue)
				Logger::Log(LogLevel::Fatal, "Accessing Optional<T> value with no value.");
		}

		return m_Value;
	}

}