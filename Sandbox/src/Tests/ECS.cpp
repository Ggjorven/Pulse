#include "ECS.hpp"

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Core/Defines.hpp>

#include <Pulse/ECS/EnTT.hpp>

enum class MyEnum : uint8_t
{
	None = 0, First = 1, Second = 2, Third = 3
};

ECSTest::ECSTest()
{
}

ECSTest::~ECSTest()
{
}

TestResult ECSTest::Execute()
{
	using namespace Pulse;

	TestResult result;

    ECS::Registry registry = {};
    PULSE_TEST(!((u32)registry.create() == 0u));

	return result;
}
