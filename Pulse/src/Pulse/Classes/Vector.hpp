#pragma once

#include <vector>
#include <ranges>
#include <functional>
#include <initializer_list>

#include "Pulse/Core/Core.hpp"
#include "Pulse/Classes/Views.hpp"

namespace Pulse
{

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	// Vector Functions
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
#if defined(PULSE_PLATFORM_WINDOWS) // Vector is not supported on Linux yet
	template<typename T>
	class Vector
	{
	public:
		// Defines
		using Allocator = std::allocator<T>;

		using Alty = std::_Rebind_alloc_t<Allocator, T>;
		using AltyTraits = std::allocator_traits<Alty>;

		using Pointer = typename AltyTraits::pointer;
		using CPointer = typename AltyTraits::const_pointer;
		using SizeType = typename AltyTraits::size_type;
		using DifferenceType = typename AltyTraits::difference_type;

		using ScaryVal = std::_Vector_val<std::conditional_t<std::_Is_simple_alloc_v<Alty>, std::_Simple_types<T>,
			std::_Vec_iter_types<T, SizeType, DifferenceType, Pointer, CPointer>>>;

		using Iterator = std::_Vector_iterator<ScaryVal>;
		using CIterator = std::_Vector_const_iterator<ScaryVal>;
		using RIterator = std::reverse_iterator<Iterator>;
		using CRIterator = std::reverse_iterator<CIterator>;

	public:
		// Constructors
		constexpr Vector() = default;
		constexpr Vector(size_t size);
		constexpr Vector(size_t size, const T& initialValue);
		constexpr Vector(const std::vector<T>& vector);
		constexpr Vector(const std::initializer_list<T>& list);
		constexpr ~Vector() = default;

		// Operators
		constexpr Vector<T>& operator = (const std::vector<T>& vector);
		constexpr Vector<T>& operator = (const std::initializer_list<T>& list);
		constexpr Vector<T>& operator = (const Vector<T>& vector);

		constexpr bool operator == (const std::vector<T>& vector);
		constexpr bool operator == (const std::initializer_list<T>& list);
		constexpr bool operator == (const Vector<T>& vector);

		constexpr T& operator [] (size_t index);

		// Iterators
		constexpr Iterator begin();
		constexpr CIterator begin() const;

		constexpr Iterator end();
		constexpr CIterator end() const;

		constexpr RIterator rbegin();
		constexpr CRIterator rbegin() const;

		constexpr RIterator rend();
		constexpr CRIterator rend() const;

		constexpr CIterator cbegin() const;
		constexpr CIterator cend() const;
		constexpr CRIterator crbegin() const;
		constexpr CRIterator crend() const;

		// Methods
		constexpr size_t Size() const;
		constexpr size_t Capacity() const;

		constexpr bool Empty() const;

		constexpr void Clear();

		constexpr void Reserve(size_t capacity);
		constexpr void Resize(size_t size);

		constexpr T* Data();

		constexpr void Push(const T& value);
		constexpr void Emplace(const T& value);
		constexpr void Emplace(const T& value, size_t index);

		constexpr void Pop();

		// Custom Methods
		using TranformFunction = std::function<T(T)>;
		[[ nodiscard ]] Views::TransformView<std::vector<T>, TranformFunction> Transform(TranformFunction function);

	private:
		std::vector<T> m_Vector;
	};



	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Vector<T>::Vector(size_t size)
		: m_Vector(size) {}

	template<typename T>
	inline constexpr Vector<T>::Vector(size_t size, const T& initialValue)
		: m_Vector(size, initialValue) {}

	template<typename T>
	inline constexpr Vector<T>::Vector(const std::vector<T>& vector)
		: m_Vector(vector) {}

	template<typename T>
	inline constexpr Vector<T>::Vector(const std::initializer_list<T>& list)
		: m_Vector(list) {}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Vector<T>& Vector<T>::operator = (const std::vector<T>& vector)
	{
		m_Vector = vector;
		return *this;
	}

	template<typename T>
	inline constexpr Vector<T>& Vector<T>::operator = (const std::initializer_list<T>& list)
	{
		m_Vector = list;
		return *this;
	}

	template<typename T>
	inline constexpr Vector<T>& Vector<T>::operator = (const Vector<T>& vector)
	{
		m_Vector = vector.m_Vector;
		return *this;
	}

	template<typename T>
	inline constexpr bool Vector<T>::operator == (const std::vector<T>& vector)
	{
		return m_Vector == vector;
	}

	template<typename T>
	inline constexpr bool Vector<T>::operator == (const std::initializer_list<T>& list)
	{
		return m_Vector == std::vector(list);
	}

	template<typename T>
	inline constexpr bool Vector<T>::operator == (const Vector<T>& vector)
	{
		return m_Vector == vector.m_Vector;
	}

	template<typename T>
	inline constexpr T& Vector<T>::operator [] (size_t index)
	{
		return m_Vector[index];
	}

	///////////////////////////////////////////////////////////
	// Iterators
	///////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Vector<T>::Iterator Vector<T>::begin() { return m_Vector.begin(); }

	template<typename T>
	inline constexpr Vector<T>::CIterator Vector<T>::begin() const { return m_Vector.begin(); }

	template<typename T>
	inline constexpr Vector<T>::Iterator Vector<T>::end() { return m_Vector.end(); }

	template<typename T>
	inline constexpr Vector<T>::CIterator Vector<T>::end() const { return m_Vector.end(); }

	template<typename T>
	inline constexpr Vector<T>::RIterator Vector<T>::rbegin() { return m_Vector.rbegin(); }

	template<typename T>
	inline constexpr Vector<T>::CRIterator Vector<T>::rbegin() const { return m_Vector.rbegin(); }

	template<typename T>
	inline constexpr Vector<T>::RIterator Vector<T>::rend() { return m_Vector.rend(); }

	template<typename T>
	inline constexpr Vector<T>::CRIterator Vector<T>::rend() const { return m_Vector.rend(); }

	template<typename T>
	inline constexpr Vector<T>::CIterator Vector<T>::cbegin() const { return m_Vector.cbegin(); }

	template<typename T>
	inline constexpr Vector<T>::CIterator Vector<T>::cend() const { return m_Vector.end(); }

	template<typename T>
	inline constexpr Vector<T>::CRIterator Vector<T>::crbegin() const { return m_Vector.crbegin(); }

	template<typename T>
	inline constexpr Vector<T>::CRIterator Vector<T>::crend() const { return m_Vector.crend(); }

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr size_t Vector<T>::Size() const { return m_Vector.size(); }

	template<typename T>
	inline constexpr size_t Vector<T>::Capacity() const { return m_Vector.capacity(); }

	template<typename T>
	inline constexpr bool Vector<T>::Empty() const { return m_Vector.empty(); }

	template<typename T>
	inline constexpr void Vector<T>::Clear() { return m_Vector.clear(); }

	template<typename T>
	inline constexpr void Vector<T>::Reserve(size_t capacity) { m_Vector.reserve(capacity); }

	template<typename T>
	inline constexpr void Vector<T>::Resize(size_t size) { m_Vector.resize(size); }

	template<typename T>
	inline constexpr T* Vector<T>::Data() { return m_Vector.data(); }

	template<typename T>
	inline constexpr void Vector<T>::Push(const T& value) { m_Vector.push_back(value); }

	template<typename T>
	inline constexpr void Vector<T>::Emplace(const T& value) { m_Vector.emplace_back(value); }

	template<typename T>
	inline constexpr void Vector<T>::Emplace(const T& value, size_t index) { m_Vector.emplace(m_Vector.begin() + index, value); }

	template<typename T>
	inline constexpr void Vector<T>::Pop() { m_Vector.pop_back(); }

	///////////////////////////////////////////////////////////
	// Custom Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	Views::TransformView<std::vector<T>, typename Vector<T>::TranformFunction> Vector<T>::Transform(TranformFunction function)
	{
		return Views::TransformView(m_Vector, function);
	}
#endif

}