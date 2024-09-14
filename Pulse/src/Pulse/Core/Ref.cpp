#include "plpch.h"
#include "Ref.hpp"

#include "Pulse/Core/Logging.hpp"

#include <mutex>
#include <thread>

namespace Pulse
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace 
	{
		static std::unordered_set<void*> s_LiveReferences = { };
		static std::mutex s_LiveReferenceMutex = {};
	}

	namespace RefUtils 
	{

		void AddToLiveReferences(void* instance)
		{
			std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);

			s_LiveReferences.insert(instance);
		}

		void RemoveFromLiveReferences(void* instance)
		{
			std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);

			if (s_LiveReferences.find(instance) == s_LiveReferences.end())
			{
				Logger::Log(LogLevel::Fatal, "Ref: Trying to remove instance which doesn't exists.");
				return;
			}

			s_LiveReferences.erase(instance);
		}

		bool IsLive(void* instance)
		{
			return s_LiveReferences.contains(instance);
		}

		std::mutex& GetMutex()
		{
			return s_LiveReferenceMutex;
		}

	}

}