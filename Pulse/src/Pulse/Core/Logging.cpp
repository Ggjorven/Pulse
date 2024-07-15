#include "plpch.h"
#include "Logging.hpp"

namespace Pulse
{

	LogFunction Logger::s_LogCallback = nullptr;

	void Logger::Init(LogFunction callback)
	{
		s_LogCallback = callback;
	}

}