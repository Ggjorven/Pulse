#include "plpch.h"
#include "TestSuite.hpp"

#include "Pulse/Core/Logging.hpp"

#include "Pulse/Text/Format.hpp"

namespace Pulse
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace
	{
		// Note: We use functions to make sure that the memory is initialized the first time we call it.
		// If we use just static variables they are not initialized in time.
		static std::string& GetRunningTest()
		{
			static std::string runningTest = "None";
			return runningTest;
		}

		static std::unordered_map<std::string, TestFn>& GetTests()
		{
			static std::unordered_map<std::string, TestFn> tests = {};
			return tests;
		}

		static std::unordered_map<std::string, TestResult>& GetTestResults()
		{
			static std::unordered_map<std::string, TestResult> results = {};
			return results;
		}
	}

	void TestSuite::Run()
	{
		size_t testID = 1;
		for (const auto& [name, func] : GetTests())
		{
			// Set the currently running test
			GetRunningTest() = name;
			
			// Run the test
			func();
			
			// Check results
			TestResult& result = GetTestResults()[name];
			if (result.Message.empty())
				Logger::Log(LogLevel::Info, "[{0}/{1}] Test '{2}' passed.", testID++, GetTests().size(), name);
			else
				Logger::Log(LogLevel::Error, "[{0}/{1}] Test '{2}' failed.\n\tFail message: {3}", testID++, GetTests().size(), name, result.Message);
		}
	}

	void TestSuite::_AddTest(const std::string& name, TestFn test)
	{
		GetTests().insert(std::make_pair(name, std::move(test)));
	}

	void TestSuite::_TestEq(bool result, const char* expr, const char* file, int line)
	{
		TestResult& testResult = GetTestResults()[GetRunningTest()];
		
		if (result == false)
			testResult.Message = Text::Format("{0} failed. \n\tFile: {1}:{2}", expr, file, line);
	}

	void TestSuite::_TestResult(bool result, const char* expr, const char* file, int line)
	{
		TestResult& testResult = GetTestResults()[GetRunningTest()];

		if (result == false)
			testResult.Message = Text::Format("{0} failed. \n\tFile: {1}:{2}", expr, file, line);
	}

}