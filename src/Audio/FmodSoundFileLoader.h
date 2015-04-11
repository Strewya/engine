#pragma once
/********************************************
*  contents:   FmodSoundLoader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
#include <Audio/FmodInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class FmodSound;

   class FmodSoundFileLoader
   {
   public:
      bool init(FMOD::System* system);
      bool shutdown();

      FmodSound load(const std::string& filename) const;

      void unload(FmodSound& texture) const;

   private:
      FMOD::System* m_system;
   };
}
