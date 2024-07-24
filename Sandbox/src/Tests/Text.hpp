#pragma once

#include <string>

#include "Tests/Test.hpp"

class TextTest : public Test
{
public:
	TextTest();
	~TextTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Text";
};