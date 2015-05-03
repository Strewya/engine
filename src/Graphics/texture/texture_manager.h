#pragma once
/********************************************
*  contents:   texture resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/texture/texture.h"
#include "graphics/texture/texture_file_loader.h"
#include "graphics/texture/texture_handle.h"
#include "util/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<Texture, HTexture, TextureFileLoader> TextureManager;
}
