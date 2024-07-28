#pragma once

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <algorithm>
#include <functional>

#include "Pulse/Classes/Vector.hpp"
#include "Pulse/Classes/HashMap.hpp"

#include "Pulse/Types/Concepts.hpp"

#include "Pulse/Events/Event.hpp"

namespace Pulse::Events
{

    class EventDispatcher
    {
    public:
        template<typename EventType>
        using EventHandler = std::function<void(const EventType&)>;

    public:
        // Constructors
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        // Methods
        template<typename EventType>
        void AddCallback(EventHandler<EventType> callback) requires Types::Concepts::InheritsFrom<Event, EventType>;
        template<typename EventType>
        void RemoveCallback(EventHandler<EventType> callback) requires Types::Concepts::InheritsFrom<Event, EventType>;

        template<typename EventType>
        void Dispatch(const EventType& e) requires Types::Concepts::InheritsFrom<Event, EventType>;

    private:
        HashMap<std::type_index, Vector<std::function<void(const Event&)>>> m_Callbacks = { };
    };
    
    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template<typename EventType>
    void EventDispatcher::AddCallback(EventHandler<EventType> handler) requires Types::Concepts::InheritsFrom<Event, EventType>
    {
        if (!m_Callbacks.Contains(StaticEventType<EventType>::ID))
            m_Callbacks.Insert(StaticEventType<EventType>::ID, { });

        m_Callbacks[StaticEventType<EventType>::ID].PushBack([handler](const Event& event)
        {
            handler(static_cast<const EventType&>(event));
        });
    }

    template<typename EventType>
    void EventDispatcher::RemoveCallback(EventHandler<EventType> callback) requires Types::Concepts::InheritsFrom<Event, EventType>
    {
    }


    template<typename EventType>
    void EventDispatcher::Dispatch(const EventType& e) requires Types::Concepts::InheritsFrom<Event, EventType>
    {
        auto list = m_Callbacks.Find(StaticEventType<EventType>::ID);
        if (list)
        {
            for (auto& callback : *list)
                callback(e);
        }
    }

}