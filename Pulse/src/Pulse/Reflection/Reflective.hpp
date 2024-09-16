#pragma once

#include "Pulse/Core/Defines.hpp"

#include "Pulse/Core/Ref.hpp"

#include "Pulse/Reflection/ClassRegistry.hpp"

#include <string>

namespace Pulse::Reflection
{

	// Interface from which all classes should inherit
	class Reflective : public RefCounted
	{
	public:
		Reflective() = default;
		virtual ~Reflective() = default;

		// TODO: Add more member functionality

		template<typename ...Args>
		static Ref<Reflective> Create(const std::string& className, Args&& ...args)
		{
			return ClassRegistry::Get().Instantiate(className, { std::any_cast<Args>(args)... });
		}
	};

}