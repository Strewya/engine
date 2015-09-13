//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "audio/audio_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/memory.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   bool AudioSystem::init(LinearAllocator& a, u32 fmodMemorySize, u32 maxSoundSlots)
   {
      CORE_INIT_START(AudioSystem);

      m_channel = nullptr;
      m_musicPlaying = HSound{};

      m_allocator.size = Megabytes(100);
      m_allocator.tag = "Audio allocator";
      m_allocator.allocated = 0;
      m_allocator.memory = allocate(a, m_allocator.size, 1);

      u8* fmodMemory = allocate(m_allocator, fmodMemorySize, 1);

      CORE_ASSERT_DBGERR(fmodMemorySize % 512 == 0, "FMOD memory size is not a multiple of 512!");
      CORE_STATUS_AND(FMOD::Memory_Initialize(fmodMemory, fmodMemorySize, 0, 0, 0) == FMOD_OK);
      CORE_STATUS_AND(FMOD::System_Create(&m_system) == FMOD_OK);
      CORE_STATUS_AND(m_system->init(32, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);

      CORE_STATUS_AND(m_fileLoader.init(m_system));
      CORE_STATUS_AND(sounds.init(m_allocator, maxSoundSlots));

      CORE_INIT_END;
   }

   // #todo the big question is do i need to do this anyway? the memory is going away anyway... probably need to shut down fmod...
   bool AudioSystem::shutdown()
   {
      CORE_SHUTDOWN_START(AudioSystem);

      if( m_channel != nullptr )
      {
         CORE_STATUS_AND(m_channel->stop() == FMOD_OK);
         m_channel = nullptr;
      }

      CORE_STATUS_AND(sounds.getCount() == 0);
      CORE_STATUS_AND(m_system->release() == FMOD_OK);
      int curAlloc = 0;
      int maxAlloc = 0;
      FMOD::Memory_GetStats(&curAlloc, &maxAlloc);
      
      CORE_LOG_DEBUG(m_allocator, "\n\tFMOD system max allocation: ", byteSizes(maxAlloc));
      CORE_SHUTDOWN_END;
   }

   HSound AudioSystem::loadSoundFromFile(const char* filename)
   {
      Sound loadedSound = m_fileLoader.load(filename);
      HSound handle = sounds.insert(loadedSound);
      return handle;
   }

   void AudioSystem::unload(HSound handle)
   {
      if( !handle.isNull() )
      {
         Sound sound = sounds.remove(handle);
         m_fileLoader.unload(sound);
      }
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
