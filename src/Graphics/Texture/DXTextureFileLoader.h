#pragma once
/********************************************
*  contents:   DXTextureLoader
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class DXTexture;

   class DXTextureFileLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      DXTexture load(const std::string& filename) const;

      void unload(DXTexture& texture) const;

   private:
      ID3D11Device* m_dev;
   };
}
