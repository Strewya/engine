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
   struct Sound;

   struct SoundFileLoader
   {
   public:
      bool init(FMOD::System* system);

      Sound load(const char* filename) const;

      void unload(Sound& sound) const;

   private:
      FMOD::System* m_system;
   };
}
