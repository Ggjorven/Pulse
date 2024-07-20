#include "Core.hpp"

#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>

struct TestClass : public Pulse::RefCounted
{
public:
	TestClass()
	{

	}
};

CoreTest::CoreTest()
{
}

CoreTest::~CoreTest()
{
}

TestResult CoreTest::Execute()
{
	using namespace Pulse;

	bool failed = false;

	// Ref Test
	void* ptr = nullptr;
	{
		Ref<TestClass> ref = Ref<TestClass>::Create();
		ptr = ref.Raw();

		failed |= !RefUtils::IsLive(ref.Raw());

		{
			Ref<TestClass> ref2 = ref;
			failed |= !RefUtils::IsLive(ref.Raw());
		}
	}
	failed |= RefUtils::IsLive(ptr);

	// Unique Test // TODO: ...
	{

	}
	
	// WeakRef Test
	{
		WeakRef<TestClass> weak;
		{
			Ref<TestClass> ref = Ref<TestClass>::Create();
			weak = ref;

			failed |= !weak.IsValid();

			Ref<TestClass> strong = weak.GetRef();
		}
		failed |= weak.IsValid();
	}

	return { .Succeeded = !failed };
}
