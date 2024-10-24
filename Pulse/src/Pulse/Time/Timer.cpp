#include "plpch.h"
#include "Timer.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse::Time
{

    Timer::Timer(bool start)
        : m_Start(Now()), m_Running(start) {}

    void Timer::Start()
    {
        if (!m_Running)
        {
            m_Start = Now();
            m_Running = true;
        }
    }

    void Timer::Stop() 
    {
        if (m_Running) 
        {
            m_End = Now();
            m_Running = false;
        }
    }

    void Timer::Reset() 
    {
        m_Start = Now();

        if (!m_Running)
            m_Running = true;
    }

    double Timer::ElapsedMillis() const 
    {
        return Elapsed<Duration, std::milli>().count();
    }

    double Timer::ElapsedSeconds() const 
    {
        return ElapsedMillis() / 1000.0;
    }

}