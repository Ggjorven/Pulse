#include "plpch.h"
#include "Logging.hpp"

#include "Pulse/Core/Core.hpp"

namespace Pulse
{

	LogFunction Logger::s_LogCallback = nullptr;
	AssertFunction Logger::s_AssertCallback = nullptr;

	static void DefaultAssertCallback(bool success, std::string message)
	{
		if (success) return;

		Logger::Log(LogLevel::Fatal, "Assertion failed: {0}", message);
		PULSE_DEBUG_BREAK();
	}

	void Logger::Init(LogFunction logCallback, AssertFunction assertCallback)
	{
		s_LogCallback = logCallback;
		s_AssertCallback = assertCallback;
	}

}