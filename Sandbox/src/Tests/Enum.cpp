#include "Enum.hpp"

#include <Pulse/Core/Logging.hpp>

#include <Pulse/Enum/Enum.hpp>
#include <Pulse/Types/TypeUtils.hpp>

enum class MyEnum : uint8_t
{
	None = 0, First = 1, Second = 2, Third = 3
};

EnumTest::EnumTest()
{
}

EnumTest::~EnumTest()
{
}

TestResult EnumTest::Execute()
{
	using namespace Pulse;

	TestResult result;

	PULSE_TEST(!(Enum::TypeName<MyEnum>() == "MyEnum"));
	PULSE_TEST(!(Enum::Name(MyEnum::First) == "First"));
	PULSE_TEST(!(Enum::Name(MyEnum::Second) == "Second"));
	PULSE_TEST(!(Enum::Name(MyEnum::Third) == "Third"));

	PULSE_TEST(!(Enum::ElementCount<MyEnum>() == 4));

	PULSE_TEST(!(Enum::Contains<MyEnum>(MyEnum::First)));
	PULSE_TEST(!(Enum::Contains<MyEnum>(1)));
	PULSE_TEST(!(Enum::Contains<MyEnum>("First")));
	PULSE_TEST(!(Enum::Contains<MyEnum>(MyEnum::Third)));
	PULSE_TEST(!(Enum::Contains<MyEnum>(3)));
	PULSE_TEST(!(Enum::Contains<MyEnum>("Third")));
	PULSE_TEST((Enum::Contains<MyEnum>(69)));
	PULSE_TEST((Enum::Contains<MyEnum>("Sixty-Nine")));

    PULSE_TEST(!(Types::Same<decltype(Enum::Fuse(MyEnum::First, MyEnum::Second)), uint16_t>));

	return result;
}
