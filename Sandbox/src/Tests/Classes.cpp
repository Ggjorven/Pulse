#include "Classes.hpp"

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Core/Loop.hpp>

#include <Pulse/Classes/Vector.hpp>
#include <Pulse/Classes/HashMap.hpp>

#include <string>

ClassesTest::ClassesTest() 
{
}

ClassesTest::~ClassesTest()
{
}

TestResult ClassesTest::Execute()
{
	using namespace Pulse;

	bool failed = false;

	// Vector class
	{
		Vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		auto filtered = vec | Vector<int>::Filter([](int n) -> bool
		{
			return !(n % 2);
		});
		auto transformed = filtered | Vector<int>::Transform([](int n) -> int
		{
			return n * 2;
		});

		// See if we can even loop over it
		Vector<int> vecFiltered = { };
		for (auto n : filtered)
			vecFiltered.PushBack(n);

		// See if we can even loop over it
		Vector<int> vecTransformed = { };
		for (auto n : transformed)
			vecTransformed.PushBack(n);
	}

	// HashMap class
	{
		HashMap<int, std::string> map = { };

		map.Insert(0, "First");
		map.Insert(1, "Second");
		map.Insert(2, "Third");

		failed |= !(map.Contains(0));
		failed |= !(map.Contains(1));
		failed |= !(map.Contains(2));
		failed |= !(map.At(0) == "First");
		failed |= !(map.At(1) == "Second");
		failed |= !(map.At(2) == "Third");
		failed |= !(map[0] == "First");
		failed |= !(map[1] == "Second");
		failed |= !(map[2] == "Third");
		failed |= (map.Contains(3));
		failed |= !(map.Find(3) == nullptr);
		
		map.Remove(0);
		failed |= (map.Contains(0));
		failed |= !(map.Find(0) == nullptr);

		// See if we can even loop over it
		HashMap<int, std::string> map2 = { };
		for (auto& [key, value] : map)
			map2.Insert(key, value);
	}

	return { .Succeeded = !failed };
}
