#pragma once

#include <string>

#include "Tests/Test.hpp"

class ThreadTest : public Test
{
public:
	ThreadTest();
	~ThreadTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Thread";
};