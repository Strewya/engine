//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/random.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Random::Random()
      : m_randEngine(std::default_random_engine()())
   {
   }

   Random::Random(uint32_t seed)
      : m_randEngine(seed)
   {
   }

   Random::Random(uint64_t seed)
      : m_randEngine(static_cast<uint32_t>(seed))
   {
   }

   void Random::reseed(uint32_t seed)
   {
      m_randEngine.seed(seed);
   }

   void Random::reseed(uint64_t seed)
   {
      m_randEngine.seed(static_cast<uint32_t>(seed));
   }

   int32_t Random::randInt(int32_t lowerBound, int32_t upperBound)
   {
      std::uniform_int_distribution<int32_t> distro(lowerBound, upperBound-1);
      return distro(m_randEngine);
   }

   float Random::randFloat()
   {
      auto max = 1000000;
      auto rnd = randInt(0, max);
      return (float)rnd / (float)max;
   }
}