#pragma once
/********************************************
*  contents:   DXTextureManager
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Texture/DXTexture.h>
#include <Graphics/Texture/DXTextureFileLoader.h>
#include <Graphics/Texture/DXTextureHandle.h>
#include <Util/Manager.h>
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<DXTexture, HTexture, DXTextureFileLoader> DXTextureManager;
}
