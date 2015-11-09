#pragma once
/********************************************
*  contents:   main audio system interface
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
   struct AudioSystem;
   struct Memory;

   namespace audio
   {
      AudioSystem* init(Memory memory, u32 fmodMemoryMegabytes, u32 fmodMaxChannels, u32 maxSoundSlots);
      void shutdown(AudioSystem* audio);

      bool frameUpdate(AudioSystem* audio);

      void playSfx(AudioSystem* sfx, HSound sound);
      void playMusic(AudioSystem* sfx, HSound sound);
      void stopMusic(AudioSystem* sfx);

      HSound loadSoundFromFile(AudioSystem* sfx, str filename);
      void unload(AudioSystem* sfx, HSound sound);
   }
}
