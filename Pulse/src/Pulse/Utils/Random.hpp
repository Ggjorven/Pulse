#pragma once

#include "Pulse/Core/Core.hpp"

#include <random>

namespace Pulse 
{

	class Random
	{
	public:
		static uint32_t UInt();
		static uint32_t UInt(uint32_t min, uint32_t max);

		static float Float();

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}

