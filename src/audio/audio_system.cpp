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
   bool AudioSystem::init(LinearAllocator& allocator, u32 audioMemorySize, u32 maxSoundSlots)
   {
      CORE_INIT_START(AudioSystem);

      m_channel = nullptr;
      m_musicPlaying = HSound{};

      u8* audioMemory = allocate(allocator, audioMemorySize, 1);

      CORE_STATUS_AND(FMOD::Memory_Initialize(audioMemory, audioMemorySize, 0, 0, 0) == FMOD_OK);
      CORE_STATUS_AND(FMOD::System_Create(&m_system) == FMOD_OK);
      CORE_STATUS_AND(m_system->init(512, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);
      
      CORE_STATUS_AND(m_fileLoader.init(m_system));
      CORE_STATUS_AND(sounds.init(allocator, m_fileLoader, maxSoundSlots));

      if( CORE_STATUS_OK )
      {
         defaultSound = m_fileLoader.load(CORE_RESOURCE("Sounds/reload.wav"));
      }

      CORE_INIT_END;
   }

   // #todo the big question is do i need to do this anyway? the memory is going away anyway...
   bool AudioSystem::shutdown()
   {
      CORE_SHUTDOWN_START(AudioSystem);
      
      if( m_channel != nullptr )
      {
         CORE_STATUS_AND(m_channel->stop() == FMOD_OK);
         m_channel = nullptr;
      }

      m_fileLoader.unload(defaultSound);
      //CORE_STATUS_AND(sounds.shutdown());
      CORE_STATUS_AND(m_fileLoader.shutdown());
      CORE_STATUS_AND(m_system->release() == FMOD_OK);
      int curAlloc = 0;
      int maxAlloc = 0;
      FMOD::Memory_GetStats(&curAlloc, &maxAlloc);
      CORE_ASSERT_DEBUG(AssertLevel::Fatal, curAlloc == 0);
      CORE_ASSERT_DEBUG(AssertLevel::Notification, maxAlloc < Megabytes(40));
      
      CORE_SHUTDOWN_END;
   }

   HSound AudioSystem::loadFromFile(const char* filename)
   {
      /*
      this function should orchestrate the following sequence of steps:
      1. a filename is provided as to the location of the sound file
      2. the filename is passed to a file loader (instantiated at the spot on the stack)
      3. the file loader simply loads the contents of the file to memory and returns the memory
      4. the memory is passed to a sound loader
      5. the sound loader parses the memory and creates the sound
      6. the sound is returned to the audio system
      7. audio system inserts the sound into cache
      8. cache returns a sound handle
      9. sound handle is mapped with the filename in a name cache
      10. sound handle is returned
      */
      u32 bufferSize = Megabytes(3);



      return HSound{};
   }

   bool AudioSystem::update()
   {
      /*
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
      }*/
      return true; // (result == FMOD_OK);
   }

   void AudioSystem::playSfx(HSound handle)
   {
//       if( !handle.isNull() )
//       {
//          auto sound = sounds.getData(handle);
//          FMOD::Channel* channel = nullptr;
//          m_system->playSound(sound._sound, nullptr, false, &channel);
//          channel->setVolume(0.5f);
//       }
   }

   void AudioSystem::playMusic(HSound handle)
   {
//       if( !handle.isNull() )
//       {
//          auto sound = sounds.getData(handle);
//          if( m_channel != nullptr )
//          {
//             m_channel->stop();
//          }
//          m_musicPlaying = handle;
//          m_system->playSound(sound._sound, nullptr, false, &m_channel);
//       }
   }

   void AudioSystem::stopMusic()
   {
//       if( m_channel != nullptr )
//       {
//          m_channel->stop();
//          m_channel = nullptr;
//       }
   }
}
