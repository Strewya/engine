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
#include "util/template/resource_manager_template.h"
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<PixelShader, HPixelShader, PixelShaderFileLoader> PixelShaderManager;
}
