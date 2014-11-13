//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Random.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Random::Random()
		: m_randEngine(std::default_random_engine()())
	{}

	Random::Random(uint32_t seed)
		: m_randEngine(seed)
	{}

	void Random::reseed(uint32_t seed)
	{
		m_randEngine.seed(seed);
	}

	int32_t Random::randInt(int32_t lowerBound, int32_t upperBound)
	{
		std::uniform_int_distribution<int32_t> distro(lowerBound, upperBound);
		return distro(m_randEngine);
	}

	float Random::randFloat()
	{
		auto max = 1000000;
		auto rnd = randInt(0, max);
		return (float)rnd/(float)max;
	}
}