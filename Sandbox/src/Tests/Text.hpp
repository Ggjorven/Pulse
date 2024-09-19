#pragma once

#include <Pulse/Core/Defines.hpp>

#include <Pulse/Test/Test.hpp>

#include <Pulse/Text/Format.hpp>

#include <Pulse/Types/TypeUtils.hpp>

TEST(TextFormat,
{
	TEST_EQ(Pulse::Text::Format("{0}", 1.0f), "1");
	TEST_EQ(Pulse::Text::Format("{0:.3f}", 1.0f), "1.000");
});