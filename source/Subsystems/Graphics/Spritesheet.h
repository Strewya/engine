#ifndef GRAPHICS_SPRITESHEET_H_
#define GRAPHICS_SPRITESHEET_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/SpriteInfo.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class Spritesheet
	{
	private:
		String _sheetName;
		String _textureName;
		uint _textureHandle;
		std::vector<SpriteInfo> _sprites;

	public:
		Spritesheet();
		Spritesheet(const char* sheetName);
		Spritesheet(const String& sheetName);
		~Spritesheet();

		uint getTextureHandle() const;
		const String& getTextureName() const;
		const String& getSpritesheetName() const;

		void setTextureHandle(uint handle);
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

#endif //GRAPHICS_SPRITESHEET_H_