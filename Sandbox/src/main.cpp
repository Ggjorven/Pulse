#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include <Pulse/Test/TestSuite.hpp>

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
	A(int a, float b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(int, float) A called with {0}, {1}", a, b);
	}

	A(float a, double b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(float, double) A called with {0}, {1}", a, b);
	}

	int Member(int a, float b)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(int, float) Member called with {0}, {1}", a, b);
		return 420;
	}

	int Member(float a)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(float) Member called with {0}", a);
		return 69;
	}

	void Member2(char a)
	{
		Pulse::Logger::Log(Pulse::LogLevel::Trace, "(char) Member2 called with {0}", a);
	}

private:
	int privateVar = 5000;

	// TODO: Make clean
	REFLECT_CLASS_CTOR(A, int, float);
	REFLECT_CLASS_CTOR(A, float, double);

	REFLECT_CLASS_MEMFN(A, Member, int, int, float);
	REFLECT_CLASS_MEMFN(A, Member, int, float);

	REFLECT_CLASS_MEMFN(A, Member2, void, char);

	REFLECT_CLASS_MEMVAR(A, int, privateVar);
};

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback);

	TestSuite::Run();

	////////////////////////////////////////
	// Reflection testing // TODO: Remove
	////////////////////////////////////////
	auto instance = Reflection::Reflective::Create("A", 54.0f, 0.0);

	return 0;
}