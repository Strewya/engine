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

		virtual InstanceID loadTexture(const char* name, const Util::Color& transparentKey, TextureData** outPtr) = 0;
		virtual TextureData* getTexture(InstanceID handle) = 0;
		virtual InstanceID getTexture(const char* name, TextureData** outPtr) = 0;
		virtual void clearTextures() = 0;
		virtual bool destroyTexture(InstanceID handle) = 0;
	};
}