#pragma once

// All precompiled-includes
#include <iostream>
#include <fstream>
#include <memory>
#include <limits>
#include <numeric>
#include <thread>
#include <chrono>
#include <future>
#include <random>
#include <utility>
#include <algorithm>
#include <functional>

#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#define PULSE_PLATFORM_WINDOWS
#elif defined(__linux__)
	#if defined(__ANDROID__)
		#define PULSE_PLATFORM_ANDROID
	#else
		#define PULSE_PLATFORM_LINUX
	#endif
#elif defined(__APPLE__)
	#define PULSE_PLATFORM_MACOS
#endif