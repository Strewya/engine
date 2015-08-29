#pragma once
/********************************************
*  contents:   fmod sound resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/fmod_sound.h"
#include "audio/fmod_sound_handle.h"
#include "audio/fmod_sound_file_loader.h"
#include "utility/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace core
{
   typedef Manager<FmodSound, HSound, FmodSoundFileLoader> SoundManager;
}
