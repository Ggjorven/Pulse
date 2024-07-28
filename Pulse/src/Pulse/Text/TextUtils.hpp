#pragma once

#include "Pulse/Classes/Vector.hpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional>

namespace Pulse::Text 
{

    Vector<std::string_view> Split(std::string_view str, char delimiter);

    std::string Join(const Vector<std::string_view>& parts, char delimiter);

    std::string_view Trim(std::string_view str);
    std::string_view LTrim(std::string_view str);
    std::string_view RTrim(std::string_view str);

}