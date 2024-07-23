#pragma once

#include <vector>
#include <ranges>
#include <functional>

#include "Pulse/Types/Traits.hpp"
#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/TypeUtils.hpp"

namespace Pulse::Loop
{

    /*
    template<Types::Concepts::Iterable Iterable>
    inline void ForEach(Iterable object, std::function<void(Types::Clean<typename Types::RangeTraits<Iterable>::RangeValue>)> callback)
    {
        for (auto element : object)
            callback(element);
    }

    template<Types::Concepts::Iterable Iterable>
    inline void ForEach(Iterable& object, std::function<void(Types::Clean<typename Types::RangeTraits<Iterable>::RangeValue>&)> callback)
    {
        for (auto& element : object)
            callback(element);
    }

    template<Types::Concepts::Iterable Iterable>
    inline void ForEach(const Iterable& object, std::function<void(const Types::Clean<typename Types::RangeTraits<Iterable>::RangeValue>&)> callback)
    {
        for (const auto& element : object)
            callback(element);
    }
    */



    inline void Range(size_t amount, std::function<void(size_t)> callback)
    {
        for (size_t i = 0ull; i < amount; i++)
            callback(i);
    }

}
