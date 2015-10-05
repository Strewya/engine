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
   void AudioSystem::init(Memory memory, u32 fmodMemoryMegabytes, u32 fmodMaxChannels, u32 maxSoundSlots)
   {
      m_channel = nullptr;
      m_musicPlaying = HSound{};

      u32 fmodMemorySize = MegaBytes(fmodMemoryMegabytes);
      CORE_ASSERT_DBGERR(fmodMemorySize % 512 == 0, "FMOD memory size has to be a multiple of 512, instead is ", fmodMemorySize % 512);

      auto fmodMemory = allocateMemoryChunk(memory, fmodMemorySize, 16);
      CORE_ASSERT_DBGERR(fmodMemory.remainingBytes >= fmodMemorySize, "Not enough memory for FMOD!");
      CORE_ASSERT_DBGWRN(fmodMemory.remainingBytes == fmodMemorySize, "Allocated more for FMOD than requested, whaaat?");
      CORE_ASSERT_DBGERR(fmodMemory != nullptr, "Failed to allocate enough memory for FMOD");

      auto result = FMOD::Memory_Initialize(fmodMemory.address, fmodMemorySize, 0, 0, 0);
      CORE_ASSERT_DBGERR(result == FMOD_OK, "Failed to initialize FMOD memory");
      result = FMOD::System_Create(&m_system);
      CORE_ASSERT_DBGERR(result == FMOD_OK, "Failed to create FMOD::System");
      result = m_system->init(fmodMaxChannels, FMOD_INIT_NORMAL, nullptr);
      CORE_ASSERT_DBGERR(result == FMOD_OK, "Failed to initialize FMOD::System");

      m_staticMemory = memory;

      m_fileLoader.init(m_system);
      sounds.init(m_staticMemory, maxSoundSlots);
   }

   void AudioSystem::shutdown()
   {
      if( m_channel != nullptr )
      {
         auto result = m_channel->stop();
         CORE_ASSERT_DBGERR(result == FMOD_OK, "Failed to stop a channel from playing");
         m_channel = nullptr;
      }

      CORE_ASSERT_DBGWRN(sounds.getCount() == 0, "Some sounds were not cleaned up!");
      auto result = m_system->release();
      CORE_ASSERT_DBGERR(result == FMOD_OK, "Failed to release FMOD::System");
      int curAlloc = 0;
      int maxAlloc = 0;
      FMOD::Memory_GetStats(&curAlloc, &maxAlloc);
      CORE_LOG_DEBUG("Audio system shutdown, stats:");
      CORE_LOG_DEBUG("FMOD system max allocation: ", byteSizes(maxAlloc));
      CORE_LOG_DEBUG("Static memory: ", m_staticMemory);
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
