#pragma once
/********************************************
*  contents:   FmodSoundManager
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Audio/FmodSound.h>
#include <Audio/FmodSoundHandle.h>
#include <Audio/FmodSoundFileLoader.h>
#include <Util/Manager.h>
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<FmodSound, HSound, FmodSoundFileLoader> FmodSoundManager;
}
