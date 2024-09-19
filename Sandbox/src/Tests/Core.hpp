#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>

#include <Pulse/Test/Test.hpp>

#include <cstdint>
#include <list>
#include <vector>

struct TestClass : public Pulse::RefCounted
{
public:
	TestClass() = default;
	virtual ~TestClass() = default;
};

struct TestClass2 : public TestClass
{
public:
	TestClass2() = default;
	~TestClass2() = default;
};



TEST(NumericSizes,
{
	TEST_EQ(Pulse::Numeric::Max<Pulse::i8>(), INT8_MAX);
	TEST_EQ(Pulse::Numeric::Max<Pulse::u64>(), UINT64_MAX);
});

TEST(WeakRef,
{
	Pulse::WeakRef<TestClass> weak;
	{
		Pulse::Ref<TestClass> ref = Pulse::Ref<TestClass>::Create();
		weak = ref;

		TEST_RESULT(weak.IsValid());

		Pulse::Ref<TestClass> strong = weak.GetRef();
	}

	TEST_RESULT(!weak.IsValid());
});

TEST(Ref,
{
	void* ptr = nullptr;
	{
		Pulse::Ref<TestClass> ref = Pulse::Ref<TestClass>::Create();
		ptr = ref.Raw();

		TEST_RESULT(Pulse::RefUtils::IsLive(ptr));

		{
			Pulse::Ref<TestClass> ref2 = ref;
			TEST_RESULT(Pulse::RefUtils::IsLive(ptr));
		}

		// Static test to see if polymorphism is allowed
		Pulse::Ref<TestClass> ref3 = Pulse::Ref<TestClass2>::Create();
	}

	TEST_RESULT(!Pulse::RefUtils::IsLive(ptr));
});
