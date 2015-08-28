//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/random.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   Random::Random()
      : m_randEngine(std::default_random_engine()())
   {}

   Random::Random(uint32_t seed)
      : m_randEngine(seed)
   {}

   Random::Random(uint64_t seed)
      : m_randEngine(static_cast<uint32_t>(seed))
   {}

   void Random::reseed(uint32_t seed)
   {
      m_randEngine.seed(seed);
   }

   void Random::reseed(uint64_t seed)
   {
      m_randEngine.seed(static_cast<uint32_t>(seed));
   }

   int32_t Random::randInt(int32_t a, int32_t b)
   {
      std::uniform_int_distribution<int32_t> distro(a, b);
      auto result = distro(m_randEngine);
      return result;
   }

   int32_t Random::randIndex(int32_t a, int32_t b)
   {
      auto result = randInt(a, b - 1);
      return result;
   }

   float Random::randFloat()
   {
      std::uniform_real_distribution<float> distro(0.0f, 1.0f);
      auto result = distro(m_randEngine);
      return result;
   }
}
