#include "plpch.h"
#include "Random.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse
{

	std::mt19937 Random::s_RandomEngine = std::mt19937(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;



	uint32_t Random::UInt()
	{
		return s_Distribution(s_RandomEngine);
	}

	uint32_t Random::UInt(uint32_t min, uint32_t max)
	{
		return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
	}

	float Random::Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

}