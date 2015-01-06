#pragma once
/********************************************
*	class:	AudioSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
#include <Audio/FmodInclude.h>
/******* extra headers *******/
#include <Util/ResourceFile.h>
/******* end header inclusion *******/

namespace Core
{
   class AudioSystem
   {
   public:
      bool init();
      bool shutdown();
      bool update();

      void* loadSound(ResourceFile);
      
      void playSfx(void*);
      void playMusic(void*);
      void stopMusic();

   private:
      std::vector<FMOD::Sound*> m_loadedSounds;
      FMOD::System* m_system;
      FMOD::Channel* m_channel;
      void* m_musicPlaying;
   };
}
