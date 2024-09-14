#include "plpch.h"
#include "TestSuite.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace
	{
		// Note: This is a pointer, since otherwise we can't be sure it has been initialized
		// by the time _AddTest gets called for the first time.
		static std::unordered_map<std::string, TestFn>* s_Tests = {};
	}

	void TestSuite::Run()
	{
		size_t testID = 1;
		for (const auto& [name, func] : *s_Tests)
		{
			bool result = func();
			if (result)
				Logger::Log(LogLevel::Info, "[{0}/{1}] Test '{2}' passed.", testID++, s_Tests->size(), name);
			else
				Logger::Log(LogLevel::Error, "[{0}/{1}] Test '{2}' failed.", testID++, s_Tests->size(), name);
		}
	}

	void TestSuite::_AddTest(const std::string& name, TestFn test)
	{
		// Manually initialize s_Tests
		if (s_Tests == nullptr)
			s_Tests = new std::unordered_map<std::string, TestFn>();

		s_Tests->insert(std::make_pair(name, std::move(test)));
	}

}