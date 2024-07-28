#pragma once

#include <string>

#include "Tests/Test.hpp"

class EventsTest : public Test
{
public:
	EventsTest();
	~EventsTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Events";

private:
	bool m_MouseReceived = false;
	bool m_KeyReceived = false;
};