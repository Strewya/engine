//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "audio/fmod_sound_file_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/fmod_sound.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool FmodSoundFileLoader::init(FMOD::System* system)
   {
      CORE_INIT_START(FmodSoundFileLoader);

      m_system = system;

      CORE_STATUS_AND(m_system != nullptr);

      CORE_INIT_END(FmodSoundFileLoader);
   }

   bool FmodSoundFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(FmodSoundFileLoader);

      m_system = nullptr;

      CORE_SHUTDOWN_END(FmodSoundFileLoader);
   }

   FmodSound FmodSoundFileLoader::load(const char* filename) const
   {
      FmodSound result{nullptr};
      FMOD_RESULT fr = m_system->createSound(filename, FMOD_DEFAULT, nullptr, &result._sound);
      if( fr != FMOD_OK )
      {
         CORE_LOG("Failed to load sound '", filename, "'");
      }
      return result;
   }

   void FmodSoundFileLoader::unload(FmodSound& sound) const
   {
      sound._sound->release();
      sound._sound = nullptr;
   }
}
