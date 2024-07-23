#pragma once

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
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define PULSE_PLATFORM_MACOS
	#endif
#endif

// Determine config
#if defined(_DEBUG) || defined(DEBUG)
	#define PULSE_CONFIG_DEBUG
#elif defined(NDEBUG)
	#define PULSE_CONFIG_RELEASE
#else // Default config == Release
	#define PULSE_CONFIG_RELEASE
#endif

// Check for proper C++ version
/* // Note: Version checking doesn't work yet. // TODO: ...
#if defined(_MSC_VER)
	#if _MSC_VER < 1933
		#error Pulse: Compiler doesnt support C++23
	#endif
#elif defined(__GNUC__) || defined(__clang__)
	#if __cplusplus < 202300L
		#error Pulse: Compiler doesnt support C++23
	#endif
#else
	#error Pulse: Unknown compiler, cannot determine C++23 support.
#endif
*/

// Define debug break // To disable debug breaks #define PULSE_DISABLE_ASSERT
#if !(defined(PULSE_DISABLE_ASSERT))
	#if defined(_MSC_VER)
		#define PULSE_DEBUG_BREAK() __debugbreak()
	#elif defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
		#include <signal.h>
		#define PULSE_DEBUG_BREAK() raise(SIGTRAP)
	#else
		#include <assert.h>
		#define PULSE_DEBUG_BREAK() assert(0)
	#endif
#endif