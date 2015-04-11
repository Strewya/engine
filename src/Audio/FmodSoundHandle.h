#pragma once
/********************************************
*  contents:   HSound
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Handle.h>
/******* end header inclusion *******/

namespace Core
{
   struct tag_FmodSound;
   typedef Handle<tag_FmodSound> HSound;
}
