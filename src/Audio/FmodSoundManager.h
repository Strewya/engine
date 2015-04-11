#pragma once
/********************************************
*  contents:   FmodSoundManager
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
#include <Audio/FmodInclude.h>
/******* extra headers *******/
#include <Audio/FmodSound.h>
#include <Audio/FmodSoundHandle.h>
#include <Audio/FmodSoundFileLoader.h>
#include <Util/Cache.h>
#include <Util/NameCache.h>
/******* end header inclusion *******/

namespace Core
{
   class FmodSoundManager
   {
   public:
      bool init(FMOD::System* system, FmodSound defaultData);
      bool shutdown();

      HSound loadFromFile(const std::string& filename);
      FmodSound& getData(HSound handle);
      void release(HSound handle);

   private:
      typedef Cache<FmodSound, HSound> TextureCache;
      typedef NameCache<HSound> TextureNames;

      TextureCache m_data;
      TextureNames m_names;
      FmodSound m_defaultData;
      FmodSoundFileLoader m_fileloader;
   };
}
