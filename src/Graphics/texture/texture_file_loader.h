#pragma once
/********************************************
*  contents:   texture resource file loader
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   struct Texture;

   struct TextureFileLoader
   {
   public:
      bool init(ID3D11Device* device);
      bool shutdown();

      Texture load(const char* filename) const;

      void unload(Texture& data) const;

   private:
      ID3D11Device* m_dev;
   };
}
