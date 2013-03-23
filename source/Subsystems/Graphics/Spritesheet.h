#ifndef SUBSYSTEMS_GRAPHICS_SPRITESHEET_H_
#define SUBSYSTEMS_GRAPHICS_SPRITESHEET_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/SpriteInfo.h"
#include "Subsystems/Graphics/Texture.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class Spritesheet
	{
	private:
		String _sheetName;
		String _textureName;
		Texture _texture;
		std::vector<SpriteInfo> _sprites;

	public:
		Spritesheet();
		Spritesheet(const char* sheetName);
		Spritesheet(const String& sheetName);
		~Spritesheet();

		const Texture& getTexture() const;
		const String& getTextureName() const;
		const String& getSpritesheetName() const;

		void setTexture(const Texture& handle);
		void setTextureName(const char* name);
		void setTextureName(const String& name);
		void setSpritesheetName(const char* name);
		void setSpritesheetName(const String& name);

		uint Insert(const SpriteInfo& sprite);
		int getHandle(const char* name) const;
		int getHandle(const String& name) const;
		const SpriteInfo& getSprite(uint handle) const;
		bool Valid(uint handle) const;

	};
}

#endif //SUBSYSTEMS_GRAPHICS_SPRITESHEET_H_