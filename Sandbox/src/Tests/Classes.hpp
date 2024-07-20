#pragma once

#include <string>

#include "Tests/Test.hpp"

class ClassesTest : public Test
{
public:
	ClassesTest();
	~ClassesTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Classes";
};