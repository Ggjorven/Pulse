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
	bool failed = false;

	failed |= !(Pulse::Types::Same<int, int>);
	failed |= !(Pulse::Types::InheritsFrom<Base, Derived>);
	failed |= !(Pulse::Types::Same<Pulse::Types::AddPointer<int>, int*>);
	failed |= !(Pulse::Types::Same<Pulse::Types::AddReference<int>, int&>);
	failed |= !(Pulse::Types::Same<Pulse::Types::Clean<const int&>, int>);

	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	IterateOver(vec);
	std::list<float> lst = { 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };
	IterateOver(lst);

	CustomIterable custom;
	IterateOver(custom);

	return { .Succeeded = !failed };
}
