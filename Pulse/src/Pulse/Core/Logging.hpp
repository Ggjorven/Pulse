#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <format>

namespace Pulse
{

	enum class LogLevel : uint8_t { None = 0, Trace, Info, Warn, Error, Fatal };

	typedef std::function<void(LogLevel level, std::string message)> LogFunction;

	// Note: This is an internal class
	class Logger
	{
	public:
		static void Init(LogFunction callback);

		// Note: This is an internal function
		template<typename ...TArgs>
		static void Log(LogLevel level, std::string_view fmt, TArgs ...args);

	private:
		static LogFunction s_LogCallback;
	};



	template<typename ...TArgs>
	inline static void Logger::Log(LogLevel level, std::string_view fmt, TArgs ...args)
	{
		if (!Logger::s_LogCallback) return;
		Logger::s_LogCallback(level, std::vformat(fmt, std::make_format_args(args...)));
	}

}