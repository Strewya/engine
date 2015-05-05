#pragma once
/********************************************
*  contents:   fmod sound resource object
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "audio/fmod_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   struct FmodSound
   {
      FMOD::Sound* _sound;

      bool loaded()
      {
         return _sound != nullptr;
      }

      bool unloaded()
      {
         return !loaded();
      }
   };
}
