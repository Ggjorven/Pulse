#include "plpch.h"
#include "TimeUtils.hpp"

namespace Pulse::Time
{

	TimePoint Now()
	{
		return Clock::now();
	}

	double NowInMillis() 
	{
		return duration_cast<Duration>(Now().time_since_epoch()).count();
	}

	double NowInSeconds() 
	{
		return duration_cast<duration<double>>(Now().time_since_epoch()).count();
	}

	void Sleep(double milliSeconds)
	{
		std::this_thread::sleep_for(Duration(milliSeconds));
	}

}