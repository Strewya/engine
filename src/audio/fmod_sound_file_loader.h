#pragma once
/********************************************
*  contents:   fmod sound file loader
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "audio/fmod_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct FmodSound;

   struct FmodSoundFileLoader
   {
   public:
      bool init(FMOD::System* system);
      bool shutdown();

      FmodSound load(const char* filename) const;

      void unload(FmodSound& texture) const;

   private:
      FMOD::System* m_system;
   };
}
