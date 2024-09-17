#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include <Pulse/Test/TestSuite.hpp>

#include <Pulse/Reflection/Attributes.hpp> // TODO: Remove
#include <Pulse/Reflection/Reflective.hpp> // TODO: Remove
#include <Pulse/Reflection/ClassRegistry.hpp> // TODO: Remove

// All tests
#include "Tests/Core.hpp"
#include "Tests/Enum.hpp"
#include "Tests/Text.hpp"
#include "Tests/Types.hpp"

static void LogCallback(Pulse::LogLevel level, std::string message)
{
	switch (level)
	{
	case Pulse::LogLevel::Trace:
		std::cout << "[Pulse] Trace: " << message << std::endl;
		break;
	case Pulse::LogLevel::Info:
		std::cout << "[Pulse] Info: " << message << std::endl;
		break;
	case Pulse::LogLevel::Warn:
		std::cout << "[Pulse] Warn: " << message << std::endl;
		break;
	case Pulse::LogLevel::Error:
		std::cout << "[Pulse] Error: " << message << std::endl;
		break;
	case Pulse::LogLevel::Fatal:
		std::cout << "[Pulse] Fatal: " << message << std::endl;
		break;

	default:
		break;
	}
}

class A : public Pulse::Reflection::Reflective
{
public:
	[[ PulseRefl::ExportCtor(public, A, int, float) ]]
	A(int a, float b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(int, float) A called with {0}, {1}", a, b);
	}

	[[ PulseRefl::ExportCtor(public, A, float, double) ]]
	A(float a, double b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(float, double) A called with {0}, {1}", a, b);
	}

	[[ PulseRefl::ExportMemFn(public, A, Member, int, int, float) ]]
	int Member(int a, float b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(int, float) Member called with {0}, {1}", a, b);
		return 420;
	}

	[[ PulseRefl::ExportMemFn(public, A, Member, int, float) ]]
	int Member(float a)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(float) Member called with {0}", a);
		return 69;
	}

	[[ PulseRefl::ExportMemFn(public, A, Member2, void, char) ]]
	void Member2(char a)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(char) Member2 called with {0}", a);
	}

private:
	[[ PulseRefl::ExportMemVar(public, A, int, privateVar) ]]
	int privateVar = 5000;
};

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback);

	TestSuite::Run();

	////////////////////////////////////////
	// Reflection testing // TODO: Remove
	////////////////////////////////////////
	Reflection::ReflectedClass instance = Reflection::ReflectedClass("A", 54.0f, 0.0);

	// Variables
	Logger::Log(LogLevel::Trace, "{0}", instance.Get<int>("privateVar"));
	int& privateVar = instance.Get<int>("privateVar");
	privateVar = 7;
	Logger::Log(LogLevel::Trace, "{0}", instance.Get<int>("privateVar"));
	instance.Set<int>("privateVar", 100);
	Logger::Log(LogLevel::Trace, "{0}", instance.Get<int>("privateVar"));

	// Functions
	Logger::Log(LogLevel::Trace, "{0}", instance.Run<int, float>("Member", 1.0f));
	Logger::Log(LogLevel::Trace, "{0}", instance.Run<int, int, float>("Member", 69, 71.3f));
	
	return 0;
}