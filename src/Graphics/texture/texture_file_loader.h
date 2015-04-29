#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "graphics/dx_include.h"
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

      void unload(DXTexture& data) const;

   private:
      ID3D11Device* m_dev;
   };
}
