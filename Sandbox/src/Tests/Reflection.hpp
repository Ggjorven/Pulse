#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Reflection/Attributes.hpp>
#include <Pulse/Reflection/Reflective.hpp>

#include <Pulse/Test/Test.hpp>

class ReflClass : public Pulse::Reflection::Reflective
{
public:
	[[ PulseRefl::ExportCtor(public, ReflClass, int, float) ]]
	ReflClass(int a, float b)
	{
	}

	[[ PulseRefl::ExportCtor(public, ReflClass, float, double) ]]
	ReflClass(float a, double b)
	{
	}

	[[ PulseRefl::ExportMemFn(public, ReflClass, Member, int, int, float) ]]
	int Member(int a, float b)
	{
		return 420;
	}

	[[ PulseRefl::ExportMemFn(public, ReflClass, Member, int, float) ]]
	int Member(float a)
	{
		return 69;
	}

	[[ PulseRefl::ExportMemFn(public, ReflClass, Member2, void, char) ]]
	void Member2(char a)
	{
	}

private:
	[[ PulseRefl::ExportMemVar(public, ReflClass, int, privateVar) ]]
	int privateVar = 5000;
};



TEST(Reflection,
{
	Pulse::Reflection::ReflectedClass instance = Pulse::Reflection::ReflectedClass("ReflClass", 54.0f, 0.0);

	// Variables
	int result = instance.Get<int>("privateVar");
	TEST_EQ(result, 5000);

	int& privateVar = instance.Get<int>("privateVar");
	privateVar = 7;
	result = instance.Get<int>("privateVar");
	TEST_EQ(result, 7);

	instance.Set<int>("privateVar", 100);
	result = instance.Get<int>("privateVar");
	TEST_EQ(result, 100);

	// Functions
	result = instance.Run<int>("Member", 1.0f);
	TEST_EQ(result, 69);

	result = instance.Run<int>("Member", 69, 71.3f);
	TEST_EQ(result, 420);
}); 