#pragma once
/********************************************
*  contents:   texture resource file loader
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Texture;

   struct TextureFileLoader
   {
   public:
      void init(ID3D11Device* device);

      Texture load(const char* filename) const;
      void unload(Texture& data) const;

   private:
      ID3D11Device* m_dev;
   };
}
