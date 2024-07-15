#include <iostream>
#include <variant>

#include <Pulse/Core/Logging.hpp>

#include "Tests/Tester.hpp"

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

int main(int argc, char* argv[])
{
	Pulse::Logger::Init(&LogCallback);

	Tester::RunAll();

	return 0;
}