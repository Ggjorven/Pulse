#pragma once

#include <mutex>
#include <atomic>

#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/TypeUtils.hpp"

namespace Pulse
{

	namespace RefUtils // We use this for WeakRef's
	{
		void AddToLiveReferences(void* instance);
		void RemoveFromLiveReferences(void* instance);
		bool IsLive(void* instance);

		std::mutex& GetMutex();
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	// RefCounted Class
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	template<typename T>
	class Ref;

	class RefCounted
	{
	public:
		RefCounted() = default;
		virtual ~RefCounted() = default;

	protected:
		inline void IncRefCount() const { ++m_RefCount; }
		inline void DecRefCount() const { --m_RefCount; }

		inline uint32_t GetRefCount() const { return m_RefCount.load(); }

	private:
		mutable std::atomic<uint32_t> m_RefCount = 0;

		template<typename T>
		friend class Ref;
	};

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	// Ref Class
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	template<typename T>
	class Ref
	{
	public:
		// Constructors
		Ref() requires Types::Concepts::InheritsFrom<RefCounted, T> = default;
		Ref(std::nullptr_t n) requires Types::Concepts::InheritsFrom<RefCounted, T>;
		Ref(T* instance) requires Types::Concepts::InheritsFrom<RefCounted, T>;
		Ref(const Ref<T>& other);
		template<typename T2>
		Ref(const Ref<T2>& other);
		template<typename T2>
		Ref(Ref<T2>&& other);
		~Ref();

		// Operators
		Ref& operator = (std::nullptr_t n);
		Ref& operator = (const Ref<T>& other);
		template<typename T2>
		Ref& operator = (const Ref<T2>& other);
		template<typename T2>
		Ref& operator = (Ref<T2>&& other);

		inline bool operator == (const Ref<T>& other) const { return m_Instance == other.m_Instance; }
		inline bool operator != (const Ref<T>& other) const { return !(*this == other); }

		inline operator bool() { return m_Instance != nullptr; }
		inline operator bool() const { return m_Instance != nullptr; }

		inline T* operator -> () { return m_Instance; }
		inline const T* operator -> () const { return m_Instance; }

		inline T& operator * () { return *m_Instance; }
		inline const T& operator * () const { return *m_Instance; }

		// Mehthods
		inline T* Raw() { return m_Instance; }
		inline const T* Raw() const { return m_Instance; }

		void Reset(T* instance = nullptr);

		template<typename T2>
		Ref<T2> As() const;

		template<typename... Args>
		static Ref<T> Create(Args&&... args);

		bool EqualsObject(const Ref<T>& other);

	private:
		// Private Methods
		void IncRef() const;
		void DecRef() const;

	private:
		mutable T* m_Instance = nullptr;

		template<typename T2>
		friend class Ref;
	};

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	template<typename T>
	Ref<T>::Ref(std::nullptr_t n) requires Types::Concepts::InheritsFrom<RefCounted, T>
		: m_Instance(nullptr) {}

	template<typename T>
	Ref<T>::Ref(T* instance) requires Types::Concepts::InheritsFrom<RefCounted, T>
		: m_Instance(instance)
	{
		IncRef();
	}

	template<typename T>
	Ref<T>::Ref(const Ref<T>& other)
		: m_Instance(other.m_Instance)
	{
		IncRef();
	}

	template<typename T>
	template<typename T2>
	Ref<T>::Ref(const Ref<T2>& other)
		: m_Instance((T*)other.m_Instance)
	{
		IncRef();
	}

	template<typename T>
	template<typename T2>
	Ref<T>::Ref(Ref<T2>&& other)
		: m_Instance((T*)other.m_Instance)
	{
		other.m_Instance = nullptr;
	}

	template<typename T>
	Ref<T>::~Ref()
	{
		DecRef();
	}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	template<typename T>
	Ref<T>& Ref<T>::operator = (std::nullptr_t n)
	{
		DecRef();
		m_Instance = nullptr;
		return *this;
	}

	template<typename T>
	Ref<T>& Ref<T>::operator = (const Ref<T>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;

		return *this;
	}

	template<typename T>
	template<typename T2>
	Ref<T>& Ref<T>::operator = (const Ref<T2>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;

		return *this;
	}

	template<typename T>
	template<typename T2>
	Ref<T>& Ref<T>::operator = (Ref<T2>&& other)
	{
		DecRef();

		m_Instance = other.m_Instance;

		other.m_Instance = nullptr;

		return *this;
	}

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	void Ref<T>::Reset(T* instance)
	{
		DecRef();

		m_Instance = instance;
	}

	template<typename T>
	template<typename T2>
	Ref<T2> Ref<T>::As() const
	{
		return Ref<T2>(*this);
	}

	template<typename T>
	template<typename... Args>
	Ref<T> Ref<T>::Create(Args&&... args)
	{
		return Ref<T>(new T(std::forward<Args>(args)...));
	}

	template<typename T>
	bool Ref<T>::EqualsObject(const Ref<T>& other)
	{
		if (!m_Instance || !other.m_Instance)
			return false;

		return *m_Instance == *other.m_Instance;
	}

	///////////////////////////////////////////////////////////
	// Private Methods
	///////////////////////////////////////////////////////////
	template<typename T>
	void Ref<T>::IncRef() const
	{
		if (m_Instance)
		{
			m_Instance->IncRefCount();

			RefUtils::AddToLiveReferences((void*)m_Instance);
		}
	}

	template<typename T>
	void Ref<T>::DecRef() const
	{
		if (m_Instance)
		{
			m_Instance->DecRefCount();
			if (m_Instance->GetRefCount() == 0)
			{
				delete m_Instance;

				RefUtils::RemoveFromLiveReferences((void*)m_Instance);
				
				m_Instance = nullptr;
			}
		}
	}

}