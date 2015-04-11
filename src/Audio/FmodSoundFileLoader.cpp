//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Audio/FmodSoundFileLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Audio/FmodSound.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
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

   FmodSound FmodSoundFileLoader::load(const std::string& filename) const
   {
      FmodSound result{nullptr};
      FMOD_RESULT fr = m_system->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &result.sound);
      if( fr != FMOD_OK )
      {
         CORE_INFO("Failed to load sound '", filename, "'");
      }
      return result;
   }

   void FmodSoundFileLoader::unload(FmodSound& sound) const
   {
      sound.sound->release();
   }
}
