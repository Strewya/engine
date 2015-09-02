//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "audio/sound_file_loader.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/sound.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool SoundFileLoader::init(FMOD::System* system)
   {
      CORE_INIT_START(SoundFileLoader);

      m_system = system;

      CORE_STATUS_AND(m_system != nullptr);

      CORE_INIT_END;
   }

   bool SoundFileLoader::shutdown()
   {
      CORE_SHUTDOWN_START(SoundFileLoader);

      m_system = nullptr;

      CORE_SHUTDOWN_END;
   }

   Sound SoundFileLoader::load(const char* filename) const
   {
      Sound result{nullptr};
      FMOD_RESULT fr = m_system->createSound(filename, FMOD_DEFAULT, nullptr, &result._sound);
      if( fr != FMOD_OK )
      {
         CORE_LOG("Failed to load sound '", filename, "'");
      }
      return result;
   }

   void SoundFileLoader::unload(Sound& sound) const
   {
      sound._sound->release();
      sound._sound = nullptr;
   }
}
