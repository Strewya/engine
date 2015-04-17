#pragma once
/********************************************
*  contents:   DXShaderManager
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Shader/ShaderHandle.h>
#include <Graphics/Shader/Pixel/PixelShader.h>
#include <Graphics/Shader/Pixel/PixelShaderFileLoader.h>
#include <Util/Manager.h>
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<PixelShader, HPixelShader, PixelShaderFileLoader> PixelShaderManager;
}
