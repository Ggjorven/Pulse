#pragma once

#include <string>

#include <Pulse/Types/Concepts.hpp>

#include "Tests/Test.hpp"

class TypesTest : public Test
{
public:
	TypesTest();
	~TypesTest();

	TestResult Execute() override;

public:
	inline static constinit const std::string_view Name = "Types";
};