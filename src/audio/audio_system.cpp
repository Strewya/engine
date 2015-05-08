//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "audio/audio_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   bool AudioSystem::init()
   {
      CORE_INIT_START(AudioSystem);

      m_channel = nullptr;
      m_musicPlaying = HSound{};

      CORE_STATUS_AND(FMOD::System_Create(&m_system) == FMOD_OK);

      if( CORE_STATUS_OK )
      {
         CORE_STATUS_AND(m_system->init(512, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);
      }

      CORE_STATUS_AND(m_fileLoader.init(m_system));

      if( CORE_STATUS_OK )
      {
         auto defaultSound = m_fileLoader.load(CORE_RESOURCE("Sounds/reload.wav"));
         CORE_STATUS_AND(sounds.init(STR(FmodSoundManager), m_fileLoader, defaultSound));
      }

      CORE_INIT_END(AudioSystem);
   }

   bool AudioSystem::shutdown()
   {
      CORE_SHUTDOWN_START(AudioSystem);

      if( m_channel != nullptr )
      {
         CORE_STATUS_AND(m_channel->stop() == FMOD_OK);
         m_channel = nullptr;
      }

      CORE_STATUS_AND(sounds.shutdown());
      CORE_STATUS_AND(m_fileLoader.shutdown());
      CORE_STATUS_AND(m_system->close() == FMOD_OK);
      CORE_STATUS_AND(m_system->release() == FMOD_OK);

      CORE_SHUTDOWN_END(AudioSystem);
   }

   bool AudioSystem::update()
   {
      FMOD_RESULT result = m_system->update();
      if( result == FMOD_OK && m_channel != nullptr )
      {
         bool playing = false;
         result = m_channel->isPlaying(&playing);
         if( !playing )
         {
            m_channel = nullptr;
            playMusic(m_musicPlaying);
         }
      }
      return (result == FMOD_OK);
   }

   void AudioSystem::playSfx(HSound handle)
   {
      if( !handle.isNull() )
      {
         auto sound = sounds.getData(handle);
         FMOD::Channel* channel = nullptr;
         m_system->playSound(sound._sound, nullptr, false, &channel);
         channel->setVolume(0.5f);
      }
   }

   void AudioSystem::playMusic(HSound handle)
   {
      if( !handle.isNull() )
      {
         auto sound = sounds.getData(handle);
         if( m_channel != nullptr )
         {
            m_channel->stop();
         }
         m_musicPlaying = handle;
         m_system->playSound(sound._sound, nullptr, false, &m_channel);
      }
   }

   void AudioSystem::stopMusic()
   {
      if( m_channel != nullptr )
      {
         m_channel->stop();
         m_channel = nullptr;
      }
   }
}
