#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Test/Test.hpp>

using namespace Pulse;

TEST(NumericSize1, 
{
	TEST_EQ(Numeric::Max<i8>(), INT8_MAX);
});

TEST(NumericSize2,
{
	TEST_EQ(Numeric::Max<i8>(), 0);
});