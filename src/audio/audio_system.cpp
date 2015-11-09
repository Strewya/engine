//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "audio/audio_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/fmod_include.h"
#include "audio/sound.h"
#include "audio/sound_cache.h"
#include "audio/sound_file_loader.h"
#include "audio/sound_handle.h"
#include "utility/memory.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{

   /************************************************************************
    *              STRUCTS
    ************************************************************************/

   struct SoundCache
   {
      // [Struja 26.9.2015.] #todo: Replace this raw array with a pool allocator based container
      // so i can more easily track how much sounds are actually in use
      Sound* storage;
      u32 maxSlots;
      u32 usedSlots;
   };

   struct AudioSystem
   {
      Memory memory;

      FMOD::System* fmodSystem;
      FMOD::Channel* fmodChannel;

      SoundCache soundCache;

      HSound musicPlaying;
   };

   /************************************************************************
    *              STATIC FUNCTIONS
    ************************************************************************/

   core_internal Sound loadSound(FMOD::System* system, str filename)
   {
      Sound result{nullptr};

      FMOD_RESULT fr = system->createSound(filename, FMOD_DEFAULT, nullptr, &result._sound);
      if( fr != FMOD_OK )
      {
         CORE_LOG("Failed to load sound '", filename, "'");
      }
      return result;
   }

   core_internal void unload(Sound& sound)
   {
      if( sound._sound )
      {
         sound._sound->release();
         sound._sound = nullptr;
      }
   }

   /************************************************************************
    *              PUBLIC FUNCTIONS
    ************************************************************************/
   namespace audio
   {
      AudioSystem* init(Memory memory, u32 fmodMemoryMegabytes, u32 fmodMaxChannels, u32 maxSoundSlots)
      {
         auto* sfx = emplace<AudioSystem>(memory);
         if( !sfx )
         {
            CORE_LOG("Not enough memory for Audio subsystem!");
            return nullptr;
         }

         sfx->memory = memory;

         sfx->fmodChannel = nullptr;
         sfx->musicPlaying = HSound{};

         u32 fmodMemorySize = MegaBytes(fmodMemoryMegabytes);
         if( fmodMemorySize % 512 != 0 )
         {
            CORE_LOG("FMOD memory size has to be a multiple of 512, instead is ", fmodMemorySize % 512);
            return nullptr;
         }

         auto fmodMemory = allocateMemoryChunk(sfx->memory, fmodMemorySize, 16);
         if( fmodMemory == nullptr )
         {
            CORE_LOG("Failed to allocate enough memory for FMOD!");
            return nullptr;
         }

         auto fmodResult = FMOD::Memory_Initialize(fmodMemory.address, fmodMemorySize, 0, 0, 0);
         if( fmodResult != FMOD_OK )
         {
            CORE_LOG("Failed to initialize FMOD memory");
            return nullptr;
         }

         fmodResult = FMOD::System_Create(&sfx->fmodSystem);
         if( fmodResult != FMOD_OK )
         {
            CORE_LOG("Failed to create FMOD::System");
            return nullptr;
         }
         fmodResult = sfx->fmodSystem->init(fmodMaxChannels, FMOD_INIT_NORMAL, nullptr);
         if( fmodResult != FMOD_OK )
         {
            CORE_LOG("Failed to initialize FMOD::System");
            return nullptr;
         }

         initSoundCache(&sfx->soundCache, sfx->memory, maxSoundSlots);

         return sfx;
      }

      void shutdown(AudioSystem* sfx)
      {
         auto fmodResult = FMOD_OK;
         if( sfx->fmodChannel != nullptr )
         {
            fmodResult = sfx->fmodChannel->stop();
            CORE_ASSERT_DBGWRN(fmodResult == FMOD_OK, "Failed to stop a channel from playing");
            sfx->fmodChannel = nullptr;
         }

         CORE_ASSERT_DBGWRN(cache::getUsedCount(&sfx->soundCache) == 0, "Some sounds were not cleaned up!");

         fmodResult = sfx->fmodSystem->release();
         CORE_ASSERT_DBGERR(fmodResult == FMOD_OK, "Failed to release FMOD::System");

         int curAlloc = 0;
         int maxAlloc = 0;
         FMOD::Memory_GetStats(&curAlloc, &maxAlloc);
         CORE_LOG_DEBUG("Audio system shutdown, stats:");
         CORE_LOG_DEBUG("FMOD system max allocation: ", byteSizes(maxAlloc));
         CORE_LOG_DEBUG("Static memory: ", sfx->memory);
      }

      bool frameUpdate(AudioSystem* sfx)
      {
         FMOD_RESULT result = sfx->fmodSystem->update();
         if( result == FMOD_OK && sfx->fmodChannel != nullptr )
         {
            bool playing = false;
            result = sfx->fmodChannel->isPlaying(&playing);
            if( !playing )
            {
               sfx->fmodChannel = nullptr;
               audio::playMusic(sfx, sfx->musicPlaying);
            }
         }
         return (result == FMOD_OK);
      }


      HSound loadSoundFromFile(AudioSystem* sfx, str filename)
      {
         Sound loadedSound = loadSound(sfx->fmodSystem, filename);
         HSound handle = cache::insert(&sfx->soundCache, loadedSound);
         return handle;
      }

      void unload(AudioSystem* sfx, HSound handle)
      {
         if( !handle.isNull() )
         {
            Sound sound = cache::remove(&sfx->soundCache, handle);
            unload(sound);
         }
      }


      void playSfx(AudioSystem* sfx, HSound handle)
      {
         if( !handle.isNull() )
         {
            auto sound = cache::get(&sfx->soundCache, handle);
            FMOD::Channel* channel = nullptr;
            sfx->fmodSystem->playSound(sound._sound, nullptr, false, &channel);
            channel->setVolume(0.5f);
         }
      }

      void playMusic(AudioSystem* sfx, HSound handle)
      {
         if( !handle.isNull() )
         {
            auto sound = cache::get(&sfx->soundCache, handle);
            if( sfx->fmodChannel != nullptr )
            {
               sfx->fmodChannel->stop();
            }
            sfx->musicPlaying = handle;
            sfx->fmodSystem->playSound(sound._sound, nullptr, false, &sfx->fmodChannel);
         }
      }

      void stopMusic(AudioSystem* sfx)
      {
         if( sfx->fmodChannel != nullptr )
         {
            sfx->fmodChannel->stop();
            sfx->fmodChannel = nullptr;
         }
      }
   }

   void init(SoundCache* cache, Memory& mem, u32 maxSlots)
   {
      cache->storage = emplaceArray<Sound>(mem, maxSlots);
      CORE_ASSERT_DBGERR(cache->storage != nullptr, "Not enough memory to emplace SoundCache storage.");
      cache->maxSlots = maxSlots;
      cache->usedSlots = 0;
   }

   namespace cache
   {
      HSound insert(SoundCache* cache, Sound s)
      {
         HSound result{};
         if( cache->maxSlots != cache->usedSlots )
         {
            for( u16 i = 0; i < cache->maxSlots; ++i )
            {
               if( isUnloaded(cache->storage[i]) )
               {
                  cache->storage[i] = s;
                  result.init(i);
                  ++cache->usedSlots;
                  break;
               }
            }
         }
         return result;
      }

      Sound remove(SoundCache* cache, HSound handle)
      {
         Sound result = cache->storage[handle.getIndex()];
         cache->storage[handle.getIndex()] = {};
         --cache->usedSlots;
         return result;
      }

      Sound get(SoundCache* cache, HSound handle)
      {
         Sound result = cache->storage[handle.getIndex()];
         return result;
      }

      u32 getUsedCount(SoundCache* cache)
      {
         return cache->usedSlots;
      }

      HSound find(SoundCache* cache, u32 assetId)
      {
         HSound result{};
         for( u16 i = 0u; i < cache->maxSlots; ++i )
         {
            if( getAssetId(cache->storage[i]) == assetId )
            {
               result.init(i);
               break;
            }
         }
         return result;
      }
   }
}
