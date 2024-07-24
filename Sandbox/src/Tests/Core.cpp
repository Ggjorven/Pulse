#include "Core.hpp"

#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>

struct TestClass : public Pulse::RefCounted
{
public:
	TestClass() {}
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

	TestResult result;

	// Ref Test
	void* ptr = nullptr;
	{
		Ref<TestClass> ref = Ref<TestClass>::Create();
		ptr = ref.Raw();

		PULSE_TEST(!RefUtils::IsLive(ref.Raw()));

		{
			Ref<TestClass> ref2 = ref;
			PULSE_TEST(!RefUtils::IsLive(ref.Raw()));
		}
	}
	PULSE_TEST(RefUtils::IsLive(ptr));

	// Unique Test // TODO: ...
	{

	}
	
	// WeakRef Test
	{
		WeakRef<TestClass> weak;
		{
			Ref<TestClass> ref = Ref<TestClass>::Create();
			weak = ref;

			PULSE_TEST(!weak.IsValid());

			Ref<TestClass> strong = weak.GetRef();
		}
		PULSE_TEST(weak.IsValid());
	}

	return result;
}
