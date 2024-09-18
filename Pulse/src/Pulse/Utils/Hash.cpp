#include "plpch.h"
#include "Hash.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse
{

	void Hash::Mix(u64& seed, u64 value)
	{
		// 0x9e3779b9 is the golden ratio of 2^32
		seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

}