#pragma once
/********************************************
*  contents:   pixel shader resource manager
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/shader/shader_handle.h"
#include "graphics/shader/pixel/pixel_shader.h"
#include "graphics/shader/pixel/pixel_shader_file_loader.h"
#include "utility/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace core
{
   typedef Manager<PixelShader, HPixelShader, PixelShaderFileLoader> PixelShaderManager;
}
