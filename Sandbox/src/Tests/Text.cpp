#include "Text.hpp"

#include <Pulse/Text/Format.hpp>
#include <Pulse/Text/TextUtils.hpp>

TextTest::TextTest()
{
}

TextTest::~TextTest()
{
}

TestResult TextTest::Execute()
{
	using namespace Pulse;

	TestResult result;

	// Format
	{
		PULSE_TEST(!(Text::Format("{0}", 1.0f) == "1"));
		PULSE_TEST(!(Text::Format("{0:.3f}", 1.0f) == "1.000"));
	}

	// Utils
	{
		std::string text = "  Hello, world!  ";
		PULSE_TEST(!(Text::Trim(text) == "Hello, world!"));

		std::string text2 = "one,two,three,four";
		auto parts = Text::Split(text2, ',');
		PULSE_TEST(!(Text::Join(parts, '-') == "one-two-three-four"));
	}

	return result;
}
