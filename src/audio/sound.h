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

   inline bool isLoaded(Sound s)
   {
      auto result = (s._sound != nullptr);
      return result;
   }

   inline bool isUnloaded(Sound s)
   {
      auto result = (s._sound == nullptr);
      return result;
   }

   inline u32 getAssetId(Sound s)
   {
      auto result = s.assetId;
      return result;
   }
}
