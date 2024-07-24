#pragma once

#include <string>

#include "Tests/Test.hpp"

class TimeTest : public Test
{
public:
	TimeTest();
	~TimeTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Time";
};