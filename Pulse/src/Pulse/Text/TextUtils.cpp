#include "plpch.h"
#include "TextUtils.hpp"

namespace Pulse::Text
{

#if defined(TEXT_UTILS_USE_STD)
    std::vector<std::string_view> Split(std::string_view str, char delimiter)
    {
        std::vector<std::string_view> result = {};

        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string_view::npos)
        {
            result.push_back(str.substr(start, end - start));

            start = end + 1;
            end = str.find(delimiter, start);
        }
        result.push_back(str.substr(start));

        return result;
    }

    std::string Join(const std::vector<std::string_view>& parts, char delimiter)
    {
        std::string result = {};

        for (size_t i = 0; i < parts.size(); ++i)
        {
            result.append(parts[i]);
            if (i != parts.size() - 1)
                result.push_back(delimiter);
        }

        return result;
    }
#else
    Vector<std::string_view> Split(std::string_view str, char delimiter)
    {
        Vector<std::string_view> result = {};

        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string_view::npos)
        {
            result.PushBack(str.substr(start, end - start));

            start = end + 1;
            end = str.find(delimiter, start);
        }
        result.PushBack(str.substr(start));

        return result;
    }

    std::string Join(const Vector<std::string_view>& parts, char delimiter)
    {
        std::string result = {};

        for (size_t i = 0; i < parts.Size(); ++i)
        {
            result.append(parts[i]);
            if (i != parts.Size() - 1)
                result.push_back(delimiter);
        }

        return result;
    }
#endif

    std::string_view LTrim(std::string_view str)
    {
        size_t start = 0;

        while (start < str.size() && std::isspace(str[start]))
            ++start;

        return str.substr(start);
    }

    std::string_view RTrim(std::string_view str)
    {
        size_t end = str.size();

        while (end > 0 && std::isspace(str[end - 1]))
            --end;

        return str.substr(0, end);
    }

    std::string_view Trim(std::string_view str)
    {
        return LTrim(RTrim(str));
    }

}
