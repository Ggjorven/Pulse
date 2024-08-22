#include "plpch.h"
#include "Thread.hpp"

namespace Pulse
{

	// Note: All threading functionality is currently stripped from macos
	// due to compilation errors with xcode.
#if defined(PULSE_PLATFORM_WINDOWS) || defined(PULSE_PLATFORM_LINUX)
	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////
	Thread::Thread(Thread&& other) noexcept
		: m_Thread(std::move(other.m_Thread)) {}

	Thread::~Thread()
	{
		if (m_Thread.joinable())
			m_Thread.join();
	}

	///////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////
	Thread& Thread::operator = (Thread&& other) noexcept
	{
		if (this != &other) 
		{
			if (m_Thread.joinable())
				m_Thread.join();
			
			m_Thread = std::move(other.m_Thread);
		}

		return *this;
	}

	///////////////////////////////////////////////////////////
	// Methods
	///////////////////////////////////////////////////////////
	void Thread::Join()
	{
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void Thread::Detach()
	{
		if (m_Thread.joinable())
			m_Thread.detach();
	}
#endif

}