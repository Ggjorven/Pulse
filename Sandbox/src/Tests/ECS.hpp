#pragma once

#include <string>

#include "Tests/Test.hpp"

class ECSTest : public Test
{
public:
	ECSTest();
	~ECSTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "ECS";
};
