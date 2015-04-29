#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "graphics/texture/texture_file_loader.h"
#include "graphics/texture/texture_handle.h"
#include "util/resource_manager_template.h"
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<DXTexture, HTexture, DXTextureFileLoader> DXTextureManager;
}
