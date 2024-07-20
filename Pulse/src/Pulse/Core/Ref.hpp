#pragma once

#include <mutex>
#include <atomic>

#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/TypeUtils.hpp"

// Non-RefCounted doesn't work yet
#define PULSE_USE_REFCOUNTED

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
	class RefCounted;

#if defined(PULSE_USE_REFCOUNTED)
	template<typename T>
	concept DerivedFromRefCounted = std::is_base_of_v<RefCounted, T>;

	#define REF_TEMPLATE(name) template<DerivedFromRefCounted name>
#else
	#define REF_TEMPLATE(name) template<typename name>
#endif

#if defined(PULSE_USE_REFCOUNTED)

	REF_TEMPLATE(T)
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

		REF_TEMPLATE(T)
		friend class Ref;
	};
#endif

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	// Ref Class
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	REF_TEMPLATE(T)
	class Ref
	{
	public:
		// Constructors
		Ref() = default;
		Ref(std::nullptr_t n);
		Ref(T* instance);
		Ref(const Ref<T>& other);
		REF_TEMPLATE(T2)
		Ref(const Ref<T2>& other);
		REF_TEMPLATE(T2)
		Ref(Ref<T2>&& other);
		~Ref();

		// Operators
		Ref& operator = (std::nullptr_t n);
		Ref& operator = (const Ref<T>& other);
		REF_TEMPLATE(T2)
		Ref& operator = (const Ref<T2>& other);
		REF_TEMPLATE(T2)
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

		REF_TEMPLATE(T2)
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

		REF_TEMPLATE(T2)
		friend class Ref;
	};

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	REF_TEMPLATE(T)
	Ref<T>::Ref(std::nullptr_t n)
		: m_Instance(nullptr) {}

	REF_TEMPLATE(T)
	Ref<T>::Ref(T* instance)
		: m_Instance(instance)
	{
		IncRef();
	}

	REF_TEMPLATE(T)
	Ref<T>::Ref(const Ref<T>& other)
		: m_Instance(other.m_Instance)
#if !(defined(PULSE_USE_REFCOUNTED))
		, m_RefCount(other.m_RefCount)
#endif
	{
		IncRef();
	}

	REF_TEMPLATE(T)
	REF_TEMPLATE(T2)
	Ref<T>::Ref(const Ref<T2>& other)
		: m_Instance((T*)other.m_Instance)
#if !(defined(PULSE_USE_REFCOUNTED))
		, m_RefCount(other.m_RefCount)
#endif
	{
		IncRef();
	}

	REF_TEMPLATE(T)
	REF_TEMPLATE(T2)
	Ref<T>::Ref(Ref<T2>&& other)
		: m_Instance((T*)other.m_Instance)
#if !(defined(PULSE_USE_REFCOUNTED))
		, m_RefCount(other.m_RefCount)
#endif
	{
		other.m_Instance = nullptr;
	}

	REF_TEMPLATE(T)
	Ref<T>::~Ref()
	{
		DecRef();
	}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	REF_TEMPLATE(T)
	Ref<T>& Ref<T>::operator = (std::nullptr_t n)
	{
		DecRef();
		m_Instance = nullptr;
		return *this;
	}

	REF_TEMPLATE(T)
	Ref<T>& Ref<T>::operator = (const Ref<T>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;
#if !(defined(PULSE_USE_REFCOUNTED))
		m_RefCount = other.m_RefCount;
#endif

		return *this;
	}

	REF_TEMPLATE(T)
	REF_TEMPLATE(T2)
	Ref<T>& Ref<T>::operator = (const Ref<T2>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;
#if !(defined(PULSE_USE_REFCOUNTED))
		m_RefCount = other.m_RefCount;
#endif

		return *this;
	}

	REF_TEMPLATE(T)
	REF_TEMPLATE(T2)
	Ref<T>& Ref<T>::operator = (Ref<T2>&& other)
	{
		DecRef();

		m_Instance = other.m_Instance;
#if !(defined(PULSE_USE_REFCOUNTED))
		m_RefCount = other.m_RefCount;
#endif

		other.m_Instance = nullptr;

#if !(defined(PULSE_USE_REFCOUNTED))
		other.m_RefCount = nullptr;
#endif

		return *this;
	}

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	REF_TEMPLATE(T)
	void Ref<T>::Reset(T* instance)
	{
		DecRef();

		m_Instance = instance;
	}

	REF_TEMPLATE(T)
	REF_TEMPLATE(T2)
	Ref<T2> Ref<T>::As() const
	{
		return Ref<T2>(*this);
	}

	REF_TEMPLATE(T)
	template<typename... Args>
	Ref<T> Ref<T>::Create(Args&&... args)
	{
		return Ref<T>(new T(std::forward<Args>(args)...));
	}

	REF_TEMPLATE(T)
	bool Ref<T>::EqualsObject(const Ref<T>& other)
	{
		if (!m_Instance || !other.m_Instance)
			return false;

		return *m_Instance == *other.m_Instance;
	}

	///////////////////////////////////////////////////////////
	// Private Methods
	///////////////////////////////////////////////////////////
#if !(defined(PULSE_USE_REFCOUNTED))
	template<typename T>
	void Ref<T>::IncRef() const
	{
		if (m_Instance)
		{
			if (!m_RefCount)
			{
				m_RefCount = new std::atomic<uint32_t>();
				*m_RefCount = 0;
			}

			++(*m_RefCount);

			RefUtils::AddToLiveReferences((void*)m_Instance);
		}
	}

	template<typename T>
	void Ref<T>::DecRef() const
	{
		if (m_Instance && m_RefCount)
		{
			--(*m_RefCount);

			if (*m_RefCount == 0)
			{
				RefUtils::RemoveFromLiveReferences((void*)m_Instance);

				delete m_Instance;
				delete m_RefCount;

				m_Instance = nullptr;
				m_RefCount = nullptr;
			}
		}
	}

#else
	template<DerivedFromRefCounted T>
	void Ref<T>::IncRef() const
	{
		if (m_Instance)
		{
			m_Instance->IncRefCount();

			RefUtils::AddToLiveReferences((void*)m_Instance);
		}
	}

	template<DerivedFromRefCounted T>
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
#endif

}