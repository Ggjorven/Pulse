#pragma once

#include "Pulse/Text/Format.hpp"

#include "Pulse/Test/TestSuite.hpp"

///////////////////////////////////////////////////////////
// Function running
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

///////////////////////////////////////////////////////////
// String concat
///////////////////////////////////////////////////////////
#define PULSE_CONC_HELPER(a, b) a##_##b
#define PULSE_CONC(a, b) PULSE_CONC_HELPER(a, b)

///////////////////////////////////////////////////////////
// If statement
///////////////////////////////////////////////////////////
#define PULSE_IF(condition, zero, one) PULSE_CONC(PULSE_IF, condition)(zero, one)
#define PULSE_IF_0(zero, one) zero
#define PULSE_IF_1(zero, one) one

// Example usage
/*
	#define OPTION0 0
	#define OPTION1 1

	#define TEST(option) PULSE_IF(option, int option0 = 0, int option1 = 0)

	TEST(OPTION0); // Gives "int option0 = 0"
*/

// Three branches
#define PULSE_IF_BR_3(condition, zero, one, two) PULSE_CONC(PULSE_IF_BR_3, condition)(zero, one, two)
#define PULSE_IF_BR_3_0(zero, one, two) zero
#define PULSE_IF_BR_3_1(zero, one, two) one
#define PULSE_IF_BR_3_2(zero, one, two) two

// Use for additional branches more than 2.
#define PULSE_IF_BR(condition, amount, ...) PULSE_CONC(PULSE_IF_BR, amount)(condition, __VA_ARGS__)

// Example usage
/*
	#define PULSE_OPTION_public 0
	#define PULSE_OPTION_private 1
	#define PULSE_OPTION_protected 2

	#define TEST(access, pub, priv, prot) PULSE_IF_BR(PULSE_CONC(PULSE_OPTION, access), 3, pub, priv, prot)

	TEST(public, MYPUB, MYPRIV, MYPROT);	// MYPUB
	TEST(private, MYPUB, MYPRIV, MYPROT);	// MYPRIV
	TEST(protected, MYPUB, MYPRIV, MYPROT);	// MYPROT
*/

///////////////////////////////////////////////////////////
// Exporting/importing C/C++
///////////////////////////////////////////////////////////
#define PULSE_IMPORT __declspec(dllimport)
#define PULSE_EXPORT __declspec(dllexport)

#define PULSE_C_EXPORT extern "C"

#define PULSE_FULL_EXPORT PULSE_C_EXPORT PULSE_EXPORT