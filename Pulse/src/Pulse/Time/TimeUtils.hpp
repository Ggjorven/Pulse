#pragma once

#include "Pulse/Core/Defines.hpp"

#include <chrono>

namespace Pulse::Time
{

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using Duration = std::chrono::duration<double, std::milli>;

    // Get the current time
	TimePoint Now();

    // Makes the current thread sleep
	void Sleep(double milliSeconds);

}