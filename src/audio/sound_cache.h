#pragma once
/********************************************
*  contents:   fmod sound resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/sound_handle.h"
/******* end header inclusion *******/

namespace core
{
   struct Memory;
   struct Sound;
   struct SoundCache;


   namespace cache
   {
      void init(SoundCache* cache, Memory& mem, u32 maxSlots);
      void shutdown(SoundCache* cache);
      
      HSound insert(SoundCache* cache, Sound s);
      Sound remove(SoundCache* cache, HSound handle);
      Sound get(SoundCache* cache, HSound handle);
      u32 getUsedCount(SoundCache* cache);
      HSound find(SoundCache* cache, u32 assetId);
   }
}
