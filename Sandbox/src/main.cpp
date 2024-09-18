#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include <Pulse/Test/TestSuite.hpp>

#include <Pulse/Dll/Dll.hpp> // TODO: Remove

// All tests
#include "Tests/Core.hpp"
#include "Tests/Enum.hpp"
#include "Tests/Text.hpp"
#include "Tests/Types.hpp"
#include "Tests/Reflection.hpp"

static void LogCallback(Pulse::LogLevel level, std::string message)
{
	using enum Pulse::LogLevel;

	switch (level)
	{
	case Trace:
		std::cout << "[Pulse] Trace: " << message << std::endl;
		break;
	case Info:
		std::cout << "[Pulse] Info: " << message << std::endl;
		break;
	case Warn:
		std::cout << "[Pulse] Warn: " << message << std::endl;
		break;
	case Error:
		std::cout << "[Pulse] Error: " << message << std::endl;
		break;
	case Fatal:
		std::cout << "[Pulse] Fatal: " << message << std::endl;
		break;

	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback);

	TestSuite::Run();

    Dll dll("Sandbox/test.so");

	return 0;
}
