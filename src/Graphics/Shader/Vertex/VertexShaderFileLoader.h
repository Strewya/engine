#pragma once
/********************************************
*  contents:   DXShaderFileLoader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Shader/Vertex/VertexShaderLoader.h>
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
