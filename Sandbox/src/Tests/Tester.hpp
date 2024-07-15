#pragma once

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Types/Concepts.hpp>
#include <Pulse/Types/TypeUtils.hpp>

#include "Tests/Test.hpp"

// All tests
#include "Tests/Core.hpp"
#include "Tests/Types.hpp"

using namespace Pulse;

class Tester
{
public:
	template<typename T>
	static void Run() requires Types::Concepts::InheritsFrom<Test, T>;

	// Note: Manually updated.
	inline static void RunAll()
	{
		RunAll<CoreTest, TypesTest>();
	}

	template<typename... Tests>
	static void RunAll();
};

// Function for running multiple tests
template<typename... Tests>
inline static void Tester::RunAll()
{
	(Tester::Run<Tests>(), ...);  // Fold expression to call Run for each Test type
}

// Function for running a test
template<typename T>
inline void Tester::Run() requires Types::Concepts::InheritsFrom<Test, T>
{
	T instance = T();
	TestResult result = instance.Execute();
	
	// We assume that the Logger has been initialized.
	if (!result.Succeeded)
		Logger::Log(LogLevel::Error, "Test: {0} failed. Message: {1}", T::Name, result.Message);
	else
		Logger::Log(LogLevel::Info, "Test: {0} succeeded.", T::Name);
}