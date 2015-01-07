//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Audio/AudioSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool AudioSystem::init()
   {
      DEBUG_STATUS(false);

      m_channel = nullptr;
      
      FMOD_RESULT result = FMOD::System_Create(&m_system);
      if( result == FMOD_OK )
      {
         result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);
         status = (result == FMOD_OK);
      }

      DEBUG_INIT(AudioSystem);
   }

   bool AudioSystem::shutdown()
   {
      DEBUG_STATUS(false);

      if( m_channel != nullptr )
      {
         status = (m_channel->stop() == FMOD_OK) && status;
         m_channel = nullptr;
      }
      for( auto* snd : m_loadedSounds )
      {
         status = (snd->release() == FMOD_OK);
      }
      status = (m_system->close() == FMOD_OK) && status;
      status = (m_system->release() == FMOD_OK) && status;

      DEBUG_SHUTDOWN(AudioSystem);
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

   void* AudioSystem::loadSound(ResourceFile file)
   {
      FMOD::Sound* snd = nullptr;
      FMOD_RESULT result = m_system->createSound(file.getPath().c_str(), FMOD_DEFAULT, nullptr, &snd);
      if( result == FMOD_OK )
      {
         m_loadedSounds.emplace_back(snd);
      }
      return snd;
   }

   bool isLoaded(std::vector<FMOD::Sound*>& list, void* snd)
   {
      for( auto* ptr : list )
      {
         if( ptr == snd )
         {
            return true;
         }
      }
      return false;
   }

   void AudioSystem::playSfx(void* snd)
   {
      if( snd != nullptr && isLoaded(m_loadedSounds, snd) )
      {
         FMOD::Channel* channel = nullptr;
         m_system->playSound((FMOD::Sound*)snd, nullptr, false, &channel);
         channel->setVolume(0.5f);
      }
   }

   void AudioSystem::playMusic(void* snd)
   {
      if( snd != nullptr && isLoaded(m_loadedSounds, snd) )
      {
         if (m_channel != nullptr)
         {
            m_channel->stop();
         }
         m_musicPlaying = snd;
         m_system->playSound((FMOD::Sound*)snd, nullptr, false, &m_channel);
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
