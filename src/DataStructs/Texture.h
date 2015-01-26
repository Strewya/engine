#pragma once
/********************************************
*	class:	Texture
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Loaders/Defines.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Texture
   {
   public:
      std::string m_name;
      size_t m_fileHash;
      Vec2f m_dimensions;
      uint32_t m_rawTextureID;
   };
}
