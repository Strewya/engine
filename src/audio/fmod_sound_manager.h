#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <audio/fmod_sound.h>
#include <audio/fmod_sound_handle.h>
#include <audio/fmod_sound_file_loader.h>
#include <util/resource_manager_template.h>
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<FmodSound, HSound, FmodSoundFileLoader> FmodSoundManager;
}
