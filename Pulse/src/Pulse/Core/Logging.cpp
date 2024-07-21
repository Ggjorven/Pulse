#include "plpch.h"
#include "Logging.hpp"

namespace Pulse
{

	LogFunction Logger::s_LogCallback = nullptr;
	AssertFunction Logger::s_AssertCallback = nullptr;

	void Logger::Init(LogFunction logCallback, AssertFunction assertCallback)
	{
		s_LogCallback = logCallback;
		s_AssertCallback = assertCallback;
	}

}