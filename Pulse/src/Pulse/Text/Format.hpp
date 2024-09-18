#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <format>

namespace Pulse::Text
{

	template<typename ...TArgs>
	inline std::string Format(std::string_view fmt, const TArgs& ...args)
	{
		return std::vformat(fmt, std::make_format_args(args...));
	}

}