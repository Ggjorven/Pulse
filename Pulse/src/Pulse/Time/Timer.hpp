#pragma once

#include "Pulse/Time/TimeUtils.hpp"

namespace Pulse::Time
{

	// A simple CPU Timer class
    class Timer 
    {
    public:
        // Constructors
        Timer(bool start = true);
        ~Timer() = default;

        // Methods
        void Start();
        void Stop();
        void Reset();

        double ElapsedMillis() const;
        double ElapsedSeconds() const;

        template<typename T = Duration, typename R = std::milli>
        inline T Elapsed() const { return m_Running ? T(Now() - m_Start) : T(m_End - m_Start); }

    private:
        TimePoint m_Start;
        TimePoint m_End;
        bool m_Running;
    };


}