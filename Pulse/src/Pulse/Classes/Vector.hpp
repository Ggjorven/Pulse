#pragma once

#include <vector>
#include <ranges>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <initializer_list>

#include "Pulse/Core/Core.hpp"
#include "Pulse/Core/Logging.hpp"

namespace Pulse
{


	template<typename T>
	class Vector
	{
	public:
		using Iterator = T*;
		using ConstIterator = const T*;

		inline static constinit const size_t s_DefaultCapacity = 4ull;
	
		// Removes item when result -> true
		inline constinit static decltype(std::views::filter)& Filter = std::views::filter;
		inline constinit static decltype(std::views::transform)& Transform = std::views::transform;

	public:
		// Constructors
		constexpr Vector();
		constexpr Vector(size_t capacity);
		constexpr Vector(size_t capacity, T&& initialValue);
		constexpr Vector(const Vector<T>& other);
		constexpr Vector(const std::initializer_list<T>& elements);
		constexpr ~Vector();

		// Operators
		constexpr Vector& operator = (const Vector<T>& other);
		constexpr Vector& operator = (const std::initializer_list<T>& elements);

		constexpr T& operator [] (size_t index);
		constexpr const T& operator [] (size_t index) const;

		// Methods
		inline constexpr Iterator begin() { return m_Array; }
		inline constexpr ConstIterator begin() const { m_Array; }
		inline constexpr Iterator end() { return m_Array + m_Size; }
		inline constexpr ConstIterator end() const { return m_Array + m_Size; }

		// Custom Methods
		inline constexpr size_t Size() const { return m_Size; }
		inline constexpr size_t Capacity() const { return m_Capacity; }

		inline constexpr bool Empty() const { return (m_Size == 0ull); }

		constexpr void Clear();

		constexpr void Reserve(size_t capacity);
		constexpr void Resize(size_t size);

		inline constexpr T* Data() { return m_Array; }

		constexpr void PushBack(const T& value);
		constexpr void PushBack(T&& value); // rvalue overload

		template<typename... Args>
		constexpr void Emplace(size_t index, Args&&... args);
		template<typename... Args>
		constexpr void EmplaceBack(Args&&... args);

		constexpr void PopBack();

	private:
		// Private Methods
		constexpr void Create(size_t capacity);
		constexpr void Create(size_t capacity, T&& initialValue);
		constexpr void Copy(const T* copyFrom, size_t copySize);
		constexpr void Destroy();

	private:
		T* m_Array = nullptr;

		size_t m_Size = 0ull;
		size_t m_Capacity = 0ull;
	};

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	template<typename T>
	constexpr Vector<T>::Vector()
	{
		Create(s_DefaultCapacity);
	}

	template<typename T>
	constexpr Vector<T>::Vector(size_t capacity)
	{
		Create(capacity);
	}

	template<typename T>
	constexpr Vector<T>::Vector(size_t capacity, T&& initialValue)
	{
		// If an array exists, delete it.
		if (m_Array)
			Destroy();

		m_Array = new T[capacity];

		// Initialize elements using move constructor
		for (size_t i = 0; i < capacity; ++i)
			new (&m_Array[i]) T(std::move(initialValue)); // Move initialization

		m_Size = 0ull;
		m_Capacity = capacity;
	}

	template<typename T>
	constexpr Vector<T>::Vector(const Vector<T>& other)
	{
		Copy(other.m_Array, other.m_Size);
	}

	template<typename T>
	constexpr Vector<T>::Vector(const std::initializer_list<T>& elements)
	{
		Copy(elements.begin(), elements.size());
	}

	template<typename T>
	constexpr Vector<T>::~Vector()
	{
		if (m_Array)
			Destroy();
	}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	template<typename T>
	constexpr Vector<T>& Vector<T>::operator = (const Vector<T>& other)
	{
		Copy(other.m_Array, other.m_Size);
		return *this;
	}

	template<typename T>
	constexpr Vector<T>& Vector<T>::operator = (const std::initializer_list<T>& elements)
	{
		Copy(elements.begin(), elements.size());
		return *this;
	}

	template<typename T>
	constexpr T& Vector<T>::operator [] (size_t index)
	{
		Logger::Assert((index < m_Size), "Tried to access element at index {0}, but size is {1}", index, m_Size);

		return m_Array[index];
	}

	template<typename T>
	constexpr const T& Vector<T>::operator [] (size_t index) const
	{
		Logger::Assert((index < m_Size), "Tried to access element at index {0}, but size is {1}", index, m_Size);

		return m_Array[index];
	}

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// Custom Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	constexpr void Vector<T>::Clear()
	{
		if (m_Array)
			Destroy(); // Also clears all variables
	}

	template<typename T>
	constexpr void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity <= m_Capacity) return;

		T* newArray = new T[capacity];

		// Move existing elements to the new array (without calling the copy constructor)
		std::move(m_Array, m_Array + m_Size, newArray);

		delete[] m_Array;

		m_Array = newArray;
		m_Capacity = capacity;
	}

	template<typename T>
	constexpr void Vector<T>::Resize(size_t size) // Don't use, just use Reserve() come on.
	{
		if (size > m_Size)
		{
			if (size > m_Capacity)
				Reserve(size);

			// Default-initialize new elements
			std::uninitialized_value_construct_n(m_Array + m_Size, size - m_Size);
		}
		else if (size < m_Size)
		{
			// Destroy excess elements (calls destructor when possible)
			std::destroy_n(m_Array + m_Size, m_Size - size);
		}

		//m_Size = size;
	}

	template<typename T>
	constexpr void Vector<T>::PushBack(const T& value)
	{
		if (m_Size >= m_Capacity) 
			Resize(m_Capacity * 2);

		m_Array[m_Size] = value;
		++m_Size;
	}

	template<typename T>
	constexpr void Vector<T>::PushBack(T&& value)
	{
		if (m_Size >= m_Capacity)
			Resize(m_Capacity * 2);
		
		m_Array[m_Size] = std::move(value);
		++m_Size;
	}

	template<typename T>
	template<typename ...Args>
	constexpr void Vector<T>::Emplace(size_t index, Args&&... args)
	{
		// Check if index is valid within size
		Logger::Assert((index < m_Size), "Tried to emplace element at index {0}, but size is {1}", index, m_Size);

		if (m_Size == m_Capacity)
			Resize(m_Capacity * 2);

		// Move elements to make space for the new element
		for (size_t i = m_Size; i > index; --i)
			m_Array[i] = std::move(m_Array[i - 1]);

		// Construct the new element in place
		new (&m_Array[index]) T(std::forward<Args>(args)...);
		++m_Size;
	}

	template<typename T>
	template<typename ...Args>
	constexpr void Vector<T>::EmplaceBack(Args&&... args)
	{
		if (m_Size >= m_Capacity) 
			Resize(m_Capacity * 2);
		
		// Construct the new element in place
		new (&m_Array[m_Size]) T(std::forward<Args>(args)...);
		++m_Size;
	}

	template<typename T>
	constexpr void Vector<T>::PopBack()
	{
		if (m_Size == 0) return;

		std::destroy_at(m_Array + (m_Size - 1));
		--m_Size;
	}

	///////////////////////////////////////////////////////////
	// Private Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	constexpr void Vector<T>::Create(size_t capacity)
	{
		// If an array exists, delete it.
		if (m_Array) 
			Destroy();

		m_Array = new T[capacity];

		m_Size = 0ull;
		m_Capacity = capacity;
	}

	template<typename T>
	constexpr void Vector<T>::Create(size_t capacity, T&& initialValue)
	{
		// If an array exists, delete it.
		if (m_Array)
			Destroy();

		m_Array = new T[capacity];

		// Initialize elements using move constructor
		for (size_t i = 0; i < capacity; ++i)
			new (&m_Array[i]) T(std::move(initialValue)); // Move initialization

		m_Size = 0ull;
		m_Capacity = capacity;
	}

	template<typename T>
	constexpr void Vector<T>::Copy(const T* copyFrom, size_t copySize)
	{
		if (copyFrom == nullptr || copySize == 0)
		{
			Create(s_DefaultCapacity);
			return;
		}

		// Check if we can keep our current memory, if not allocate new memory
		if (m_Capacity < copySize)
			Create(copySize);

		std::copy(copyFrom, copyFrom + copySize, m_Array);

		m_Size = copySize;
	}

	template<typename T>
	constexpr void Vector<T>::Destroy()
	{
		delete[] m_Array;

		m_Array = nullptr;

		m_Size = 0ull;
		m_Capacity = 0ull;
	}

}