#pragma once

#include <string>

#include "Tests/Test.hpp"

class EnumTest : public Test
{
public:
	EnumTest();
	~EnumTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Enum";
};