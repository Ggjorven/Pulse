#pragma once

#include "Pulse/Classes/Vector.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional>

// If enabled uses std::vector else uses Pulse::Vector
#define TEXT_UTILS_USE_STD

namespace Pulse::Text
{

#if defined(TEXT_UTILS_USE_STD)
    std::vector<std::string_view> Split(std::string_view str, char delimiter);
    std::string Join(const std::vector<std::string_view>& parts, char delimiter);
#else
    Vector<std::string_view> Split(std::string_view str, char delimiter);
    std::string Join(const Vector<std::string_view>& parts, char delimiter);
#endif

    std::string_view Trim(std::string_view str);
    std::string_view LTrim(std::string_view str);
    std::string_view RTrim(std::string_view str);

}
