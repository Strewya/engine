#pragma once
/********************************************
*  contents:   vertex shader loader from file
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "graphics/shader/vertex/vertex_shader_loader.h"
#include "graphics/graphics_types.h"
/******* end header inclusion *******/

namespace Core
{
   class VertexShader;

   class VertexShaderFileLoader
   {
   public:
      bool init(VertexShaderLoader& loader);
      bool shutdown();

      VertexShader load(const char* filename, InputLayout layout) const;

      void unload(VertexShader& data);

   private:
      VertexShaderLoader* m_loader;
   };
}
