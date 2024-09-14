#pragma once

#include "Pulse/Text/Format.hpp"

#include "Pulse/Test/TestSuite.hpp"

///////////////////////////////////////////////////////////
// Helper macros
///////////////////////////////////////////////////////////
#define RUN_FUNCTION_HELPER(id, functionName, ...)			\
namespace													\
{															\
	inline void Run##id()									\
	{														\
		functionName(__VA_ARGS__);							\
	}														\
															\
	static int dummy##id = (Run##id(), 0);					\
}

#define RUN_FUNCTION_NN_HELPER(id, functionName, ...)		\
inline static void Run##id()								\
{															\
    functionName(__VA_ARGS__);								\
}															\
															\
inline static const int dummy##id = (Run##id(), 0);

#define RUN_FUNCTION(id, functionName, ...)				RUN_FUNCTION_HELPER(id, functionName, __VA_ARGS__)
#define RUN_FUNCTION_NONAMESPACE(id, functionName, ...) RUN_FUNCTION_NN_HELPER(id, functionName, __VA_ARGS__)

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