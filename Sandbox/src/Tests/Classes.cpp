#include "Classes.hpp"

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Classes/Vector.hpp>

ClassesTest::ClassesTest()
{
}

ClassesTest::~ClassesTest()
{
}

TestResult ClassesTest::Execute()
{
	using namespace Pulse;
	using namespace Pulse::Views;

	bool failed = false;

	// Custom
	/*
	{
		Vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		auto view = vec.Transform([](int n) -> int
		{
			return n * 2;
		});
		auto view2 = FilterView<TransformView<std::vector<int>, std::function<int(int)>>, std::function<bool(int)>>(view, [](int n) -> bool
		{
			return ((n == 2) || (n == 4));
		});

		//static_assert(Types::Concepts::Range<decltype(view)>);
		//auto res1 = std::ranges::begin(view);
		//auto res2 = std::ranges::end(view);

		//for (auto a : view2)
		//	Logger::Log(LogLevel::Trace, "{0}", a);
	}
	*/

	Logger::Log(LogLevel::Trace, "-------");

	// Default
	{
		std::vector<int> vecOld = { 1, 2, 3, 4 };
		auto view = vecOld | std::views::transform([](int n) -> int { return n * 2; });
		auto view2 = view | std::views::filter([](int n) -> bool { return ((n == 2) || (n == 4)); });

		for (auto a : view2)
			Logger::Log(LogLevel::Trace, "{0}", a);
	}

	return { .Succeeded = !failed };
}
