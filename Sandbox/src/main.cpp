#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include "Tests/Tester.hpp"

// All tests
#include "Tests/Core.hpp"
#include "Tests/Text.hpp"
#include "Tests/Types.hpp"
#include "Tests/Events.hpp"
#include "Tests/ECS.hpp"
#include "Tests/Classes.hpp"
#include "Tests/Enum.hpp"
#include "Tests/Thread.hpp"
#include "Tests/Time.hpp"

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
	}
}

static void AssertCallback(bool success, std::string message)
{
	if (success) return;

	Pulse::Logger::Log(LogLevel::Fatal, "Assertion failed: {0}", message);
	PULSE_DEBUG_BREAK();
}

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback, &AssertCallback);

	Tester::Run<CoreTest>();
	Tester::Run<TextTest>();
	Tester::Run<TypesTest>();
	Tester::Run<EventsTest>();
	Tester::Run<ECSTest>();
	Tester::Run<ClassesTest>();
	Tester::Run<EnumTest>();
	Tester::Run<ThreadTest>();
	Tester::Run<TimeTest>();

	return 0;
}
