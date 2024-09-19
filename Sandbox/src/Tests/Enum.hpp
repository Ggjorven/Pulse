#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Test/Test.hpp>

#include <Pulse/Enum/Enum.hpp>

#include <Pulse/Types/TypeUtils.hpp>

enum class MyEnum : uint8_t
{
	None = 0, First = 1, Second = 2, Third = 3
};



TEST(Enum,
{
	TEST_EQ(Pulse::Enum::TypeName<MyEnum>(), "MyEnum");
	TEST_EQ(Pulse::Enum::Name(MyEnum::First), "First");
	TEST_EQ(Pulse::Enum::Name(MyEnum::Second), "Second");
	TEST_EQ(Pulse::Enum::Name(MyEnum::Third), "Third");

	TEST_EQ(Pulse::Enum::ElementCount<MyEnum>(), 4);

	TEST_RESULT(Pulse::Enum::Contains<MyEnum>(MyEnum::First));
	TEST_RESULT(Pulse::Enum::Contains<MyEnum>(1));
	TEST_RESULT(Pulse::Enum::Contains<MyEnum>("First"));
	TEST_RESULT(Pulse::Enum::Contains<MyEnum>(MyEnum::Third));
	TEST_RESULT(Pulse::Enum::Contains<MyEnum>(3));
	TEST_RESULT(Pulse::Enum::Contains<MyEnum>("Third"));
	TEST_RESULT(!Pulse::Enum::Contains<MyEnum>(69));
	TEST_RESULT(!Pulse::Enum::Contains<MyEnum>("Sixty-Nine"));
});