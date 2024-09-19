#include "plpch.h"
#include "TimeUtils.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse::Time
{

	TimePoint Now()
	{
		return Clock::now();
	}

	void Sleep(double milliSeconds)
	{
		std::this_thread::sleep_for(Duration(milliSeconds));
	}

}