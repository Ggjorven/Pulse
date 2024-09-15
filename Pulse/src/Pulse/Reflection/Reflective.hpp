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
	};

}