#include "Enum.hpp"

#include <Pulse/Core/Logging.hpp>

#include <Pulse/Enum/Enum.hpp>

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

	bool failed = false;

	failed |= !(Enum::TypeName<MyEnum>() == "MyEnum");
	failed |= !(Enum::Name(MyEnum::First) == "First");
	failed |= !(Enum::Name(MyEnum::Second) == "Second");
	failed |= !(Enum::Name(MyEnum::Third) == "Third");

	failed |= !(Enum::ElementCount<MyEnum>() == 4);

	failed |= !(Enum::Contains<MyEnum>(MyEnum::First));
	failed |= !(Enum::Contains<MyEnum>(1));
	failed |= !(Enum::Contains<MyEnum>(MyEnum::Third));
	failed |= !(Enum::Contains<MyEnum>(3));
	failed |= (Enum::Contains<MyEnum>(69));

	return { .Succeeded = !failed };
}
