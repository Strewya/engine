#pragma once
/********************************************
*  contents:   texture resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "graphics/texture/texture_file_loader.h"
#include "graphics/texture/texture_handle.h"
#include "utility/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace core
{
   typedef Manager<Texture, HTexture, TextureFileLoader> TextureManager;
}
