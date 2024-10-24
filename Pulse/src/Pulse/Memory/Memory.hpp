#pragma once

#include "Pulse/Core/Core.hpp"
#include "Pulse/Core/Defines.hpp"
#include "Pulse/Core/Logging.hpp"

#include "Pulse/Types/Traits.hpp"
#include "Pulse/Types/Concepts.hpp"

#include <memory>

namespace Pulse::Memory
{

	class ArenaAllocator;
	class DynamicArenaAllocator;

	// Note: To give control of private constructors etc
	// make Control a friend class
	class Control
	{
	public:
		static constexpr const bool MemoryTracking = false; // Not implemented.
	public:
		template<typename T, typename ...TArgs>
		static T* ConstructAt(T* location, TArgs&& ...args)
		{
			return static_cast<T*>(new (static_cast<void*>(location)) T(std::forward<TArgs>(args)...));
		}

		template<typename TArray>
		static void DestroyArray(TArray* arr)
		{
			auto beg = std::begin(arr);
			auto end = std::end(arr);

			if constexpr (!Types::Concepts::TriviallyDestructible<typename Types::IteratorTraits<TArray>::ValueType>)
			{
				for (; beg != end; beg++)
					DestroyAt(*beg);
			}
		}

		template<typename T>
		static void DestroyAt(T* instance)
		{
			if constexpr (Types::Concepts::Array<T>) 
				DestroyArray<T>(instance);
			else
				instance->~T();
		}

	public:
		template<typename T, typename ...TArgs>
		static T* Allocate(TArgs&& ...args)
		{
			return new T(std::forward<TArgs>(args)...);
		}

		template<typename T>
		static void Free(T* instance)
		{
			delete instance;
		}

	private:
		friend class ArenaAllocator;
		friend class DynamicArenaAllocator;
	};

}