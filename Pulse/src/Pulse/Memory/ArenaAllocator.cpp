#include "plpch.h"
#include "ArenaAllocator.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse::Memory
{

	ArenaAllocator::ArenaAllocator(Pulse::u64 size)
		: m_Size(size), m_Buffer((Pulse::byte*)std::malloc(size)), m_Offset(m_Buffer)
	{
	}

	ArenaAllocator::~ArenaAllocator()
	{
		std::free((void*)m_Buffer);
	}

	void* ArenaAllocator::Allocate(Pulse::u64 size)
	{
		if constexpr (MemoryValidation)
		{
			if ((m_Offset + size) > (m_Buffer + m_Size))
			{
				Pulse::Logger::Log(Pulse::LogLevel::Fatal, "ArenaAllocator::Construct exceeds buffer size of {0} bytes.", m_Size);
				return nullptr;
			}
		}

		void* object = m_Offset;
		m_Offset += size; // Move the pointer 

		return object;
	}

}