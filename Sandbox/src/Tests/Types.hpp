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

private:
	template<Pulse::Types::Concepts::Iterable T>
	void IterateOver(const T& value);

public:
	inline static constinit const std::string_view Name = "Types";
};

// More of a static test, since it gives compilation errors
template<Pulse::Types::Concepts::Iterable T>
inline void TypesTest::IterateOver(const T& value)
{
	for (auto& obj : value) 
		continue;
}
