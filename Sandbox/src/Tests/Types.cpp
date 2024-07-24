#include "Types.hpp"

#include <list>
#include <vector>

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

TypesTest::TypesTest()
{
}

TypesTest::~TypesTest()
{
}

TestResult TypesTest::Execute()
{
	TestResult result;

	PULSE_TEST(!(Pulse::Types::Same<int, int>));
	PULSE_TEST(!(Pulse::Types::InheritsFrom<Base, Derived>));
	PULSE_TEST(!(Pulse::Types::Same<Pulse::Types::AddPointer<int>, int*>));
	PULSE_TEST(!(Pulse::Types::Same<Pulse::Types::AddReference<int>, int&>));
	PULSE_TEST(!(Pulse::Types::Same<Pulse::Types::Clean<const int&>, int>));

	PULSE_TEST(!Pulse::Types::Concepts::Iterable<std::vector<int>>);
	PULSE_TEST(!Pulse::Types::Concepts::Iterable<std::list<float>>);
	PULSE_TEST(!Pulse::Types::Concepts::Iterable<CustomIterable>);

	return result;
}
