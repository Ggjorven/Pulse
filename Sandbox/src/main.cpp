/*
#include <iostream>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Core/Logging.hpp>

#include <Pulse/Test/TestSuite.hpp>

// All tests
#include "Tests/Core.hpp"

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
*/
#include <iostream>

#include "Pulse/Reflection/ClassRegistry.hpp"

class ExampleClass : public Pulse::Reflection::IReflectable
{
    PULSE_REFLECT(ExampleClass, int, double) // Registering constructor with (int, double)
    PULSE_REFLECT(ExampleClass, int, int, int) // Registering constructor with (int, double)

public:
    ExampleClass(int a, int b, int c)
    {
        std::cout << "ExampleClass constructor called with: " << a << ", " << b << ", " << c << "\n";
    }
    ExampleClass(int a, double b)
    {
        std::cout << "ExampleClass constructor called with: " << a << ", " << b << "\n";
    }

    void DoSomething(int x)
    {
        std::cout << "DoSomething called with: " << x << "\n";
    }

    static void RegisterMemberFunctions()
    {
        Pulse::Reflection::ClassRegistry::Get().RegisterMemberFunction("ExampleClass", "DoSomething", [](IReflectable* instance, const std::vector<std::any>& args)
            {
                auto obj = static_cast<ExampleClass*>(instance);
                int x = std::any_cast<int>(args[0]);
                obj->DoSomething(x);
            });
    }
};

int main()
{
    using namespace Pulse::Reflection;

    // Register ExampleClass and its member functions
    ExampleClass::RegisterReflection();
    ExampleClass::RegisterMemberFunctions();

    // Create an instance of ExampleClass with constructor arguments (int, double)
    std::vector<std::any> constructorArgs = { 42, 3.14 };
    Pulse::Ref<IReflectable> instance = ClassRegistry::Get().CreateInstance("ExampleClass", constructorArgs);

    std::vector<std::any> constructorArgs2 = { 42, 69, 420 };
    Pulse::Ref<IReflectable> instance2 = ClassRegistry::Get().CreateInstance("ExampleClass", constructorArgs2);

    // Call member function "DoSomething" with arguments
    std::vector<std::any> functionArgs = { 100 };
    ClassRegistry::Get().CallMemberFunction("ExampleClass", "DoSomething", instance.Raw(), functionArgs);

    return 0;
}
