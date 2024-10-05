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

	DynamicArenaAllocator::DynamicArenaAllocator(Pulse::u64 size)
		: m_Size(size), m_Buffers({ (Pulse::byte*)std::malloc(size) }), m_Offsets({ m_Buffers[0] })
	{
	}

	DynamicArenaAllocator::~DynamicArenaAllocator()
	{
		for (auto& buffer : m_Buffers)
			std::free((void*)buffer);
	}

	void* DynamicArenaAllocator::Allocate(Pulse::u64 size)
	{
		if ((m_Offsets[(m_Offsets.size() - 1)] + size) > (m_Buffers[(m_Buffers.size() - 1)] + m_Size))
			AllocateBuffer(m_Size);

		void* object = m_Offsets[(m_Offsets.size() - 1)];
		m_Offsets[(m_Offsets.size() - 1)] += size; // Move the pointer 

		return object;
	}

	void DynamicArenaAllocator::AllocateBuffer(Pulse::u64 size)
	{
		Pulse::byte* block = (Pulse::byte*)std::malloc(size);

		m_Buffers.push_back(block);
		m_Offsets.push_back(block);
	}

}