#pragma once

#include "Pulse/Utils/Macros.hpp"

///////////////////////////////////////////////////////////
// Main macros
///////////////////////////////////////////////////////////
#define TEST_EQ(x, y) ::Pulse::TestSuite::_TestEq((x == y), ::Pulse::Text::Format("({0} == {1})", #x, #y).c_str(), __FILE__, __LINE__)
// Note: True mean success
#define TEST_RESULT(x) ::Pulse::TestSuite::_TestResult((x), ::Pulse::Text::Format("({0})", #x).c_str(), __FILE__, __LINE__)

#define TEST(name, body)									\
inline void _PulseTest_##name()								\
body														\
															\
RUN_FUNCTION(__COUNTER__, ::Pulse::TestSuite::_AddTest, #name, &_PulseTest_##name)