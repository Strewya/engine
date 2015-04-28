#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <graphics/shader/vertex/vertex_shader_loader.h>
/******* end header inclusion *******/

namespace Core
{
   class VertexShader;

   class VertexShaderFileLoader
   {
   public:
      bool init(VertexShaderLoader& loader);
      bool shutdown();

      VertexShader load(const std::string& filename, InputLayout layout) const;

      void unload(VertexShader& data);

   private:
      VertexShaderLoader* m_loader;
   };
}
