#pragma once

#include "Pulse/Types/Traits.hpp"
#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/TypeUtils.hpp"

#include <thread>

namespace Pulse
{

	class Thread
	{
	public:
        // Constructors
	    template<Types::Concepts::Callable F, typename ...Args>
        Thread(F&& func, Args&&... args) requires(Types::Concepts::CallableWithArgs<F, typename Types::FunctionTraits<F>::ReturnType, Args...>);
        Thread(Thread&& other) noexcept;
        Thread(const Thread&) = delete;
        ~Thread();

        
        // Operators
        Thread& operator = (const Thread&) = delete;
        Thread& operator = (Thread&& other) noexcept;

        inline operator std::thread&() { return m_Thread; }
        inline operator const std::thread&() const { return m_Thread; }

        // Methods
        void Join();
        void Detach();

    private:
        std::thread m_Thread;
	};

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template<Types::Concepts::Callable F, typename ...Args>
    Thread::Thread(F&& func, Args&&... args) requires(Types::Concepts::CallableWithArgs<F, typename Types::FunctionTraits<F>::ReturnType, Args...>)
        : m_Thread(std::thread(std::forward<F>(func), std::forward<Args>(args)...)) {}

}