#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class TextureData;
}
namespace Util
{
	class Color;
}

namespace Graphics
{
	class ITextureCache
	{
	public:
		virtual ~ITextureCache() {}

		virtual TextureData* LoadTexture(const char* name, const Util::Color& transparentKey) = 0;
		virtual TextureData* LoadTexture(const String& name, const Util::Color& transparentKey) = 0;
		virtual TextureData* getTexture(uint32_t handle) = 0;
	};
}