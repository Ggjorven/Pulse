#pragma once

#include "Pulse/Core/Core.hpp"
#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include <memory>

namespace Pulse::Memory
{

	class ArenaAllocator
	{
	public:
		constexpr static const bool MemoryValidation = true;
	public:
		explicit ArenaAllocator(Pulse::u64 size);
		~ArenaAllocator();

		void* Allocate(Pulse::u64 size);

		// Note: If constructor is private, make ArenaAllocator::Construct a friend function
		template<typename T, typename ...TArgs>
		T* Construct(TArgs&& ...args);

		template<typename T>
		void Destroy(T* object);

	private:
		const Pulse::u64 m_Size;

		Pulse::byte* m_Buffer;
		Pulse::byte* m_Offset;
	};

	class DynamicArenaAllocator
	{
	public:
		explicit DynamicArenaAllocator(Pulse::u64 size);
		~DynamicArenaAllocator();

		void* Allocate(Pulse::u64 size);

		// Note: If constructor is private, make DynamicArenaAllocator::Construct a friend function
		template<typename T, typename ...TArgs>
		T* Construct(TArgs&& ...args);

		template<typename T>
		void Destroy(T* object);

	private:
		void AllocateBuffer(Pulse::u64 size);

	private:
		const Pulse::u64 m_Size;

		std::vector<Pulse::byte*> m_Buffers;
		std::vector<Pulse::byte*> m_Offsets;
	};

	///////////////////////////////////////////////////////////
	// ArenaAllocator
	///////////////////////////////////////////////////////////
	template<typename T, typename ...TArgs>
	inline T* ArenaAllocator::Construct(TArgs&& ...args)
	{
		if constexpr (MemoryValidation)
		{
			if ((m_Offset + sizeof(T)) > (m_Buffer + m_Size))
			{
				Pulse::Logger::Log(Pulse::LogLevel::Fatal, "ArenaAllocator::Construct exceeds buffer size of {0} bytes.", m_Size);
				return nullptr;
			}
		}

		// Custom std::construct_at
		T* object = static_cast<T*>(new (static_cast<void*>(m_Offset)) T(std::forward<TArgs>(args)...));
		m_Offset += sizeof(T); // Move the pointer 

		return object;
	}

	template<typename T>
	inline void ArenaAllocator::Destroy(T* object)
	{
		std::destroy_at<T>(object);
	}

	///////////////////////////////////////////////////////////
	// DynamicArenaAllocator
	///////////////////////////////////////////////////////////
	template<typename T, typename ...TArgs>
	inline T* DynamicArenaAllocator::Construct(TArgs&& ...args)
{
		if ((m_Offsets[(m_Offsets.size() - 1)] + sizeof(T)) > (m_Buffers[(m_Buffers.size() - 1)] + m_Size))
			AllocateBuffer(m_Size);

		new (static_cast<void*>(m_Offsets[(m_Offsets.size() - 1)])) T(std::forward<TArgs>(args)...);

		// Custom std::construct_at
		T* object = static_cast<T*>(new (static_cast<void*>(m_Offsets[(m_Offsets.size() - 1)])) T(std::forward<TArgs>(args)...));
		m_Offsets[(m_Offsets.size() - 1)] += sizeof(T); // Move the pointer 

		return object;
	}

	template<typename T>
	inline void DynamicArenaAllocator::Destroy(T* object)
	{
		std::destroy_at<T>(object);
	}

}