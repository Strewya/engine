#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/AnimationInfo.h"
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
		uint _textureHandle;
		std::deque<SpriteInfo> _sprites;
		std::deque<AnimationInfo> _animations;

	public:
		Spritesheet();
		Spritesheet(const char* sheetName);
		Spritesheet(const String& sheetName);
		~Spritesheet();

		uint getTextureHandle() const;
		const String& getTextureName() const;
		const String& getSpritesheetName() const;

		void setTexture(uint textureHandle);
		void setTextureName(const char* name);
		void setTextureName(const String& name);
		void setSpritesheetName(const char* name);
		void setSpritesheetName(const String& name);

		uint Insert(const SpriteInfo& sprite);
		uint Insert(const AnimationInfo& animation);

		int getSpriteHandle(const char* name) const;
		int getSpriteHandle(const String& name) const;
		int getAnimationHandle(const char* name) const;
		int getAnimationHandle(const String& name) const;
		const SpriteInfo& getSprite(uint handle) const;
		const AnimationInfo& getAnimation(uint handle) const;
		bool SpriteValid(uint handle) const;
		bool AnimationValid(uint handle) const;

	};
}