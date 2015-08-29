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

   Random::Random(u32 seed)
      : m_randEngine(seed)
   {}

   Random::Random(u64 seed)
      : m_randEngine(static_cast<u32>(seed))
   {}

   void Random::reseed(u32 seed)
   {
      m_randEngine.seed(seed);
   }

   void Random::reseed(u64 seed)
   {
      m_randEngine.seed(static_cast<u32>(seed));
   }

   i32 Random::randInt(i32 a, i32 b)
   {
      std::uniform_int_distribution<i32> distro(a, b);
      auto result = distro(m_randEngine);
      return result;
   }

   i32 Random::randIndex(i32 a, i32 b)
   {
      auto result = randInt(a, b - 1);
      return result;
   }

   f32 Random::randFloat()
   {
      std::uniform_real_distribution<f32> distro(0.0f, 1.0f);
      auto result = distro(m_randEngine);
      return result;
   }
}
