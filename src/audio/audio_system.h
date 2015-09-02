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
#include "audio/sound_manager.h"
#include "sound_handle.h"
#include "sound_file_loader.h"
/******* end header inclusion *******/

namespace core
{
   struct LinearAllocator;

   struct AudioSystem
   {
   public:
      bool init(LinearAllocator& allocator, u32 maxSounds);
      bool shutdown();
      bool update();

      void playSfx(HSound sound);
      void playMusic(HSound sound);
      void stopMusic();

      HSound loadFromFile(const char* filename);

      SoundManager sounds;

   private:
      //allocator type
      SoundFileLoader m_fileLoader;
      FMOD::System* m_system;
      FMOD::Channel* m_channel;
      HSound m_musicPlaying;
   };
}
