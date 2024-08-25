#include "Classes.hpp"

#include <Pulse/Core/Logging.hpp>

#include <Pulse/Classes/Vector.hpp>
#include <Pulse/Classes/HashMap.hpp>

#include <Pulse/Types/Concepts.hpp>

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

	TestResult result;

	// Vector class
	{
		PULSE_TEST(!Types::Concepts::Iterable<Vector<int>>);
	}

	// HashMap class
	{
		HashMap<int, std::string> map = { };

		map.Insert(0, "First");
		map.Insert(1, "Second");
		map.Insert(2, "Third");

		PULSE_TEST(!(map.Contains(0)));
		PULSE_TEST(!(map.Contains(1)));
		PULSE_TEST(!(map.Contains(2)));

		PULSE_TEST(!(map.At(0) == "First"));
		PULSE_TEST(!(map.At(1) == "Second"));
		PULSE_TEST(!(map.At(2) == "Third"));

		PULSE_TEST(!(map[0] == "First"));
		PULSE_TEST(!(map[1] == "Second"));
		PULSE_TEST(!(map[2] == "Third"));

		PULSE_TEST((map.Contains(3)));
		PULSE_TEST(!(map.Find(3) == nullptr));

		map.Remove(0);
		PULSE_TEST((map.Contains(0)));
		PULSE_TEST(!(map.Find(0) == nullptr));

		PULSE_TEST((Types::Concepts::Iterable<HashMap<int, std::string>>));
	}

	return result;
}
