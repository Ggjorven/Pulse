#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Test/Test.hpp>

#include <Pulse/Types/TypeUtils.hpp>

struct Base {};
struct Derived : public Base {};

struct CustomIterable
{
public:
	inline bool* begin() { return m_Data; }
	inline bool* end() { return m_Data + (sizeof(bool) * m_Size); }
	inline const bool* begin() const { return m_Data; }
	inline const bool* end() const { return m_Data + (sizeof(bool) * m_Size); }

private:
	int m_Size = 5;
	bool m_Data[5] = { false, true, true, false, false };
};



TEST(Types,
{
	//TEST_RESULT(Pulse::Types::Same<int, int>);

	//TEST_RESULT(Pulse::Types::Concepts::InheritsFrom<Base, Derived>);
	TEST_RESULT(Pulse::Types::Concepts::Iterable<std::vector<int>>);
	TEST_RESULT(Pulse::Types::Concepts::Iterable<std::list<float>>);
	TEST_RESULT(Pulse::Types::Concepts::Iterable<CustomIterable>);
});