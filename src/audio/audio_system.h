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
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct AudioSystem
   {
   public:
      bool init(Allocator& a, u32 systemMemory, u32 fmodMemoryMegabytes, u32 fmodMaxChannels, u32 maxSoundSlots);
      bool shutdown();
      bool update();

      void playSfx(HSound sound);
      void playMusic(HSound sound);
      void stopMusic();

      HSound loadSoundFromFile(const char* filename);

      void unload(HSound sound);

      SoundCache sounds;

   private:
      FrameAllocator m_staticMemory;
      SoundFileLoader m_fileLoader;
      FMOD::System* m_system;
      FMOD::Channel* m_channel;
      HSound m_musicPlaying;
   };
}
