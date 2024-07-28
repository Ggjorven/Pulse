#pragma once

#include <typeinfo>
#include <typeindex>

namespace Pulse::Events
{

    class Event // Event classes mush inherit from this base class
    {
    public:
        Event() = default;
        virtual ~Event() = default;
    };

    template<typename T>
    struct StaticEventType { static const std::type_index ID; };

    template<typename T>
    const std::type_index StaticEventType<T>::ID = std::type_index(typeid(T));

}