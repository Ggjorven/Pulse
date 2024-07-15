#pragma once

#include <string>

#include "Tests/Test.hpp"

class CoreTest : public Test
{
public:
	CoreTest();
	~CoreTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Core";
};