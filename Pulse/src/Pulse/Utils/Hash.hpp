#pragma once

#include "Pulse/Core/Defines.hpp"

#include "Pulse/Types/Concepts.hpp"

namespace Pulse
{

	class Hash
	{
	public:
		// HashMixing/HashCombination algorithm by Bob Jenkins (lookup3)
		static void Mix(u64& seed, u64 value);
	};

}