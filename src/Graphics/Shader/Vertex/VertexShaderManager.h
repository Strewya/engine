#pragma once
/********************************************
*  contents:   DXShaderManager
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Shader/ShaderHandle.h>
#include <Graphics/Shader/Vertex/VertexShader.h>
#include <Graphics/Shader/Vertex/VertexShaderFileLoader.h>
#include <Util/Manager.h>
/******* end header inclusion *******/

namespace Core
{
   typedef Manager<VertexShader, HVertexShader, VertexShaderFileLoader> VertexShaderManager;
}
