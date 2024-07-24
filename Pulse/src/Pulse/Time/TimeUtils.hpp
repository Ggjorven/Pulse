#pragma once

#include <chrono>

namespace Pulse::Time
{

	using namespace std::chrono;

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using Duration = std::chrono::duration<double, std::milli>;



	// Default formats
	TimePoint Now();

	double NowInMillis();
	double NowInSeconds();

	// For custom formats
	template<typename T = Duration, typename R = std::milli>
	inline T NowCustom() { return duration_cast<T>(Now().time_since_epoch()); }

	// Makes current thread sleep
	void Sleep(double milliSeconds);

}