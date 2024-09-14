#pragma once

#include "Pulse/Core/Defines.hpp"

#include <functional>
#include <unordered_map>

namespace Pulse
{

	// Return true is success
	using TestFn = std::function<bool()>;

	class TestSuite
	{
	public:
		static void Run();

		// Internal call for adding tests
		static void _AddTest(const std::string& name, TestFn test);
	};

}