#pragma once
/********************************************
*  contents:   fmod sound resource object
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "audio/fmod_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Sound
   {
      FMOD::Sound* _sound;
      u32 assetId;
   };

   bool isLoaded(Sound s)
   {
      return s._sound != nullptr;
   }

   bool isUnloaded(Sound s)
   {
      return s._sound == nullptr;
   }

   u32 getAssetId(Sound s)
   {
      return s.assetId;
   }
}
