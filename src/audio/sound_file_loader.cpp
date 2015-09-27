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
   void SoundFileLoader::init(FMOD::System* system)
   {
      m_system = system;
      CORE_ASSERT_DBGERR(m_system != nullptr, "NULL FMOD::System passed to SoundFileLoader!");
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
      if( sound._sound )
      {
         sound._sound->release();
         sound._sound = nullptr;
      }
   }
}
