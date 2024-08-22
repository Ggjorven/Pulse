#pragma once

#include "Pulse/Core/Core.hpp"

#include <mutex>

namespace Pulse
{

    // Note: All threading functionality is currently stripped from macos
    // due to compilation errors with xcode.
#if defined(PULSE_PLATFORM_WINDOWS) || defined(PULSE_PLATFORM_LINUX)
    class Mutex 
    {
    public:
        // Operators
        inline operator std::mutex& () { return m_Mutex; }
        inline operator const std::mutex& () const { return m_Mutex; }

        // Methods
        inline void Lock() { m_Mutex.lock(); }
        inline void Unlock() { m_Mutex.unlock(); }

        inline bool TryLock() { return m_Mutex.try_lock(); }

    private:
        std::mutex m_Mutex = {};
    };
#endif

}