#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <audio/fmod_include.h>
/******* extra headers *******/
#include <audio/fmod_sound_manager.h>
/******* end header inclusion *******/

namespace Core
{
   class AudioSystem
   {
   public:
      bool init();
      bool shutdown();
      bool update();

      void playSfx(HSound sound);
      void playMusic(HSound sound);
      void stopMusic();

      FmodSoundManager sounds;

   private:
      FmodSoundFileLoader m_fileLoader;
      FMOD::System* m_system;
      FMOD::Channel* m_channel;
      HSound m_musicPlaying;
   };
}
