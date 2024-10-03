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

		template<typename T, typename ...TArgs>
		T* Construct(TArgs&& ...args);

		template<typename T>
		void Destroy(T* object);

	private:
		const Pulse::u64 m_Size;

		Pulse::byte* m_Buffer;
		Pulse::byte* m_Offset;
	};



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

		T* object = std::construct_at<T>(static_cast<T*>(static_cast<void*>(m_Offset)), std::forward<TArgs>(args)...);
		m_Offset += sizeof(T); // Move the pointer 

		return object;
	}

	template<typename T>
	inline void ArenaAllocator::Destroy(T* object)
	{
		std::destroy_at<T>(object);
	}

}