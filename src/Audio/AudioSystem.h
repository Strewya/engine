#pragma once
/********************************************
*  class:   AudioSystem
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <Audio/FmodInclude.h>
/******* extra headers *******/
#include <Audio/FmodSoundManager.h>
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
