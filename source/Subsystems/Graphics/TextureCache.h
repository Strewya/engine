#ifndef SUBSYSTEMS_GRAPHICS_TEXTURECACHE_H_
#define SUBSYSTEMS_GRAPHICS_TEXTURECACHE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/Texture.h"
	/*** end header inclusion ***/

namespace Core
{
	class ResourceLocator;
	class ServiceLocator;
}

namespace Graphics
{
	class IRenderer;

	class TextureCache
	{
	public:
		TextureCache();

		Texture& getTexture(const char* name);
		Texture& getTexture(const String& name);
		Texture& getTexture(uint handle);
		uint getTextureHandle(const char* name);
		uint getTextureHandle(const String& name);

		void setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services);

	private:
		std::deque<Texture> _textures;
		TextureCache* _parent;
		IRenderer* _gfx;
	};
}

#endif //SUBSYSTEMS_GRAPHICS_TEXTURECACHE_H_