#pragma once
/********************************************
*  contents:   DXTextureLoader
*  usage:
********************************************/
/******* C++ headers *******/
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
      DXTextureFileLoader(ID3D11Device* dev);

      DXTexture load(std::string filename);

   private:
      ID3D11Device* m_dev;
   };
}
