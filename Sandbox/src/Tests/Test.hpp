#pragma once

struct TestResult
{
public:
	bool Succeeded = true;
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