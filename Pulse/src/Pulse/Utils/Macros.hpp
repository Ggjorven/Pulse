#pragma once

#include "Pulse/Text/Format.hpp"

#include "Pulse/Test/TestSuite.hpp"

///////////////////////////////////////////////////////////
// Helper macros
///////////////////////////////////////////////////////////
#define RUN_FUNCTION_HELPER(id, functionName, ...)			\
namespace													\
{															\
	inline void _Run##id()									\
	{														\
		functionName(__VA_ARGS__);							\
	}														\
															\
	static const char _RunDummy##id = (_Run##id(), 0);		\
}

#define RUN_FUNCTION_NN_HELPER(id, functionName, ...)		\
inline static void _Run##id()								\
{															\
    functionName(__VA_ARGS__);								\
}															\
															\
inline static const char _RunDummy##id = (_Run##id(), 0)

#define RUN_FUNCTION(id, functionName, ...)				RUN_FUNCTION_HELPER(id, functionName, __VA_ARGS__)
#define RUN_FUNCTION_NN(id, functionName, ...)			RUN_FUNCTION_NN_HELPER(id, functionName, __VA_ARGS__)