#pragma once

#include "Pulse/Core/Defines.hpp"

#include <functional>
#include <unordered_map>

namespace Pulse
{

	// Return true is success
	using TestFn = std::function<void()>;

	struct TestResult
	{
	public:
		bool Success = true;
		std::string FailMessage = {};
	};

	class TestSuite
	{
	public:
		static void Run();

		// Internal calls
		static void _AddTest(const std::string& name, TestFn test);
		static void _TestEq(bool result, const char* expr, const char* file, int line);
		static void _TestResult(bool result, const char* expr, const char* file, int line);
	};

}