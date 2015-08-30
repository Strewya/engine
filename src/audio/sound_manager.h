#pragma once
/********************************************
*  contents:   fmod sound resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/sound.h"
#include "audio/sound_handle.h"
#include "audio/sound_file_loader.h"
#include "utility/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace core
{
   typedef Manager<Sound, HSound, SoundFileLoader> SoundManager;
}
