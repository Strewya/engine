#pragma once
/********************************************
*  contents:   main audio system interface
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "audio/fmod_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "audio/sound_cache.h"
#include "sound_handle.h"
#include "sound_file_loader.h"
/******* end header inclusion *******/

namespace core
{
   struct LinearAllocator;

   struct AudioSystem
   {
   public:
      bool init(u32 fmodMemoryMegabytes, u32 fmodMaxChannels, u32 maxSoundSlots);
      bool shutdown();
      bool update();

      void playSfx(HSound sound);
      void playMusic(HSound sound);
      void stopMusic();

      HSound loadSoundFromFile(const char* filename);

      void unload(HSound sound);

      SoundCache sounds;

   private:
      LinearAllocator m_staticMemory;
      SoundFileLoader m_fileLoader;
      FMOD::System* m_system;
      FMOD::Channel* m_channel;
      HSound m_musicPlaying;

      friend AudioSystem* createAudioSystem(MemoryBlock memory);
   };

   AudioSystem* createAudioSystem(MemoryBlock memory);
}
