#pragma once

struct TestResult
{
public:
	bool Failed = false;
	std::string Message = "None";
};

// Simple virtual class for running tests
class Test
{
public:
	Test() = default;
	virtual ~Test() = default;

	virtual TestResult Execute() = 0;
};

#define PULSE_TEST(x) result.Failed |= (x); result.Message = #x