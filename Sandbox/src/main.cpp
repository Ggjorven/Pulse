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
	REFLECT_CLASS_CTOR(A, int, float);
	REFLECT_CLASS_CTOR(A, float, double);

	REFLECT_CLASS_MEMFN(A, Member, int, int, float);
	REFLECT_CLASS_MEMFN(A, Member, int, float);

	REFLECT_CLASS_MEMFN(A, Member2, void, char);

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
};

int main(int argc, char* argv[])
{
	using namespace Pulse;

	Logger::Init(&LogCallback);
	
	TestSuite::Run();

	// ///////////////////////
	auto i = Reflection::ClassRegistry::Get().Instantiate("A", { 69, 420.69f });
	auto i2 = Reflection::ClassRegistry::Get().Instantiate("A", { 69.0f, 420.6999999 });

	int r = std::any_cast<int>(Reflection::ClassRegistry::Get().RunMember("A", "Member", i.Raw(), { 79, 123.45f }));
	Pulse::Logger::Log(Pulse::LogLevel::Trace, "Result value: {0}", r);

	r = std::any_cast<int>(Reflection::ClassRegistry::Get().RunMember("A", "Member", i.Raw(), { 789.12f }));
	Pulse::Logger::Log(Pulse::LogLevel::Trace, "Result value: {0}", r);

	Reflection::ClassRegistry::Get().RunMember("A", "Member2", i.Raw(), { '=' });

	return 0;
}