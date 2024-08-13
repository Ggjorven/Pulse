#pragma once

#include <cstdint>
#include <string>
#include <numeric>
#include <limits>

namespace Pulse
{

    // Rust like naming conventions
    using byte = char;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using f32 = float;
    using f64 = double;

    using str = std::string;

    namespace Numeric
    {
        // Get max value of T
        template<typename T> inline consteval T Max() { return std::numeric_limits<T>::max(); }

        template<> inline consteval i8 Max() { return INT8_MAX; }
        template<> inline consteval i16 Max() { return INT16_MAX; }
        template<> inline consteval i32 Max() { return INT32_MAX; }
        template<> inline consteval i64 Max() { return INT64_MAX; }

        template<> inline consteval u8 Max() { return UINT8_MAX; }
        template<> inline consteval u16 Max() { return UINT16_MAX; }
        template<> inline consteval u32 Max() { return UINT32_MAX; }
        template<> inline consteval u64 Max() { return UINT64_MAX; }

        template<> inline consteval f32 Max() { return std::numeric_limits<float>::max(); }
        template<> inline consteval f64 Max() { return std::numeric_limits<double>::max(); }
    }

}
