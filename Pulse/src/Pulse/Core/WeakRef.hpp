#pragma once

#include "Pulse/Core/Ref.hpp"

namespace Pulse
{

	template<typename T>
	class WeakRef
	{
	public:
		// Constructors
		WeakRef() = default;
		WeakRef(T* ptr);
		WeakRef(Ref<T> ref);
		~WeakRef() = default;

		// Operators
		inline operator bool() { return IsValid(); }
		inline operator bool() const { return IsValid(); }
		
		// Methods
		inline bool IsValid() const { return m_Instance ? RefUtils::IsLive(m_Instance) : false; }

		Ref<T> GetRef() const;

		template<typename T2>
		WeakRef<T2> As() const;

	private:
		T* m_Instance = nullptr;
	};

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	template<typename T>
	WeakRef<T>::WeakRef(T* ptr)
		: m_Instance(ptr) {}

	template<typename T>
	WeakRef<T>::WeakRef(Ref<T> ref)
		: m_Instance(ref.Raw()) {}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	Ref<T> WeakRef<T>::GetRef() const
	{
		return Ref<T>(m_Instance);
	}

	template<typename T>
	template<typename T2>
	WeakRef<T2> WeakRef<T>::As() const
	{
		return WeakRef<T2>(dynamic_cast<T2*>(m_Instance));
	}

}