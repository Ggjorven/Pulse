#include <iostream>

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Classes/Vector.hpp>

#include "Tests/Tester.hpp"

// All tests
#include "Tests/Core.hpp"
#include "Tests/Types.hpp"
#include "Tests/Classes.hpp"

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
	using namespace Pulse;

	Logger::Init(&LogCallback);

	Tester::Run<CoreTest>();
	Tester::Run<TypesTest>();
	Tester::Run<ClassesTest>();

	return 0;
}