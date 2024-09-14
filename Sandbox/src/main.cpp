#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include <Pulse/Test/TestSuite.hpp>

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

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback);
	
	TestSuite::Run();

	return 0;
}