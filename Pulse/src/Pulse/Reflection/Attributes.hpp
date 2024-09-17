#pragma once

#include "Pulse/Reflection/ClassRegistry.hpp"

// The Empty var will be stripped by the compiler (as long as you don't use it ;))
#define PULSE_EXPORT_HELPER_2(id)	Pulse::Reflection::Empty _Reflection##id = 0
#define PULSE_EXPORT_HELPER(id)		PULSE_EXPORT_HELPER_2(id)

// Example usage:
// [[ PulseRefl::ExportCtor(public, ExampleClass, int) ]]
// [[ PulseRefl::ExportCtor(public, ExampleClass, int, char*) ]]
// [[ PulseRefl::ExportMemFn(public, ExampleClass, MemberFunc, int) ]]
// [[ PulseRefl::ExportMemFn(public, ExampleClass, MemberFunc, int, char*) ]]
// [[ PulseRefl::ExportMemVar(private, ExampleClass, double, MyVariable) ]]
#define PulseRefl ]] inline static constexpr const

#define ExportCtor(access, cls, ...)								\
	PULSE_EXPORT_HELPER(__COUNTER__);								\
	PULSE_REFLECT_CLASS_CTOR(cls, __VA_ARGS__);						\
	access:															\
	[[

// Note: Currently doesn't support private/protected
#define ExportMemFn(access, cls, fnName, retType, ...)				\
	PULSE_EXPORT_HELPER(__COUNTER__);								\
	PULSE_REFLECT_CLASS_MEMFN(cls, fnName, retType, __VA_ARGS__);	\
	access:															\
	[[

#define ExportMemVar(access, cls, type, name)						\
	PULSE_EXPORT_HELPER(__COUNTER__);								\
	PULSE_REFLECT_CLASS_MEMVAR(cls, type, name);					\
	access:															\
	[[