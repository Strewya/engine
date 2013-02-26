#ifndef GRAPHICS_TEXTURECACHE_H_
#define GRAPHICS_TEXTURECACHE_H_
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

namespace Graphics
{
	class Interface;

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		Texture& getTexture(const char* name);

		void setReferences(TextureCache& parent, Interface& gfx);

	private:
		std::deque<Texture> _textures;
		TextureCache* _parent;
		Interface* _gfx;
	};
}

#endif //GRAPHICS_TEXTURECACHE_H_