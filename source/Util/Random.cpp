//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Random.h"
	/*** C++ headers ***/
#include <chrono>
	/*** extra headers ***/
	/*** end headers ***/
	
namespace Util
{
	Random::Random()
		: m_randEngine(std::default_random_engine()())
	{}

	Random::Random(uint32_t seed)
		: m_randEngine(seed)
	{}

	uint32_t Random::rand(uint32_t lowerBound, uint32_t upperBound)
	{
		std::uniform_int_distribution<uint32_t> distro(lowerBound, upperBound);
		return distro(m_randEngine);
	}
}