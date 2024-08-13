#include "Core.hpp"

#include <Pulse/Core/Logging.hpp>
#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>
#include <Pulse/Core/Defines.hpp>

struct TestClass : public Pulse::RefCounted
{
public:
	TestClass() = default;
	virtual ~TestClass() = default;
};

struct TestClass2;

// Static test to check if forward declaration works
Pulse::Ref<TestClass2> Test2Create();

struct TestClass2  : public TestClass
{
public:
	TestClass2() = default;
	~TestClass2() = default;
};

Pulse::Ref<TestClass2> Test2Create()
{
	return Pulse::Ref<TestClass2>::Create();
}

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

		// Static test to see if polymorphism is allowed
		Ref<TestClass> ref3 = Ref<TestClass2>::Create();
	}
	PULSE_TEST(RefUtils::IsLive(ptr));

	// Unique Test
	{
		// Static test to see if polymorphism is allowed
		Unique<TestClass> unique = Unique<TestClass2>::Create();
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

    // Defines
    {
        #if defined(INT32_MAX)
        PULSE_TEST(!(Numeric::Max<i32>() == INT32_MAX));
        #endif
        #if defined(UINT32_MAX)
        PULSE_TEST(!(Numeric::Max<u32>() == UINT32_MAX));
        #endif
    }

	return result;
}
