#pragma once

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Types/Concepts.hpp>
#include <Pulse/Types/TypeUtils.hpp>

#include "Tests/Test.hpp"

using namespace Pulse;

class Tester
{
public:
	template<typename T>
	static void Run() requires Types::Concepts::InheritsFrom<Test, T>;

	template<typename FirstTest, typename SecondTest, typename... Tests> // We use so many template arguments to force multiple tests
	static void Run();
};

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

// Function for running multiple tests
template<typename FirstTest, typename SecondTest, typename... Tests>
inline void Tester::Run()
{
	Tester::Run<FirstTest>();
	Tester::Run<SecondTest>();

	(Tester::Run<Tests>(), ...);  // Fold expression to call Run for each Test type
}