#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <format>

#include "Pulse/Text/Format.hpp"

namespace Pulse
{

	enum class LogLevel : uint8_t { None = 0, Trace, Info, Warn, Error, Fatal };

	typedef std::function<void(LogLevel level, std::string message)> LogFunction;
	typedef std::function<void(bool success, std::string message)> AssertFunction;

	// Note: This is an internal class
	class Logger
	{
	public:
		static void Init(LogFunction logCallback, AssertFunction assertCallback = nullptr);

		// Note: This is an internal function
		template<typename ...TArgs>
		static void Log(LogLevel level, std::string_view fmt, const TArgs& ...args);

		template<typename ...TArgs>
		static void Assert(bool success, std::string_view fmt, const TArgs& ...args);

	private:
		static LogFunction s_LogCallback;
		static AssertFunction s_AssertCallback;
	};



	template<typename ...TArgs>
	inline void Logger::Log(LogLevel level, std::string_view fmt, const TArgs& ...args)
	{
		if (!Logger::s_LogCallback) return;
		Logger::s_LogCallback(level, Text::Format(fmt, args...));
	}

	template<typename ...TArgs>
	inline void Logger::Assert(bool success, std::string_view fmt, const TArgs& ...args)
	{
		if (!Logger::s_AssertCallback) return;
		Logger::s_AssertCallback(success, Text::Format(fmt, args...));
	}

}