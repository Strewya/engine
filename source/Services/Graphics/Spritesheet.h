#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Services/Graphics/AnimationInfo.h"
#include "Services/Graphics/SpriteInfo.h"
#include "Services/Graphics/TextureData.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class Spritesheet
	{
	private:
		String _sheetName;
		String _textureName;
		uint32_t _textureHandle;
		std::deque<SpriteInfo> _sprites;
		std::deque<AnimationInfo> _animations;

	public:
		Spritesheet();
		Spritesheet(const char* sheetName);
		Spritesheet(const String& sheetName);
		~Spritesheet();

		uint32_t getTextureHandle() const;
		const String& getTextureName() const;
		const String& getSpritesheetName() const;

		void setTexture(uint32_t textureHandle);
		void setTextureName(const char* name);
		void setTextureName(const String& name);
		void setSpritesheetName(const char* name);
		void setSpritesheetName(const String& name);

		uint32_t Insert(const SpriteInfo& sprite);
		uint32_t Insert(const AnimationInfo& animation);

		int getSpriteHandle(const char* name) const;
		int getSpriteHandle(const String& name) const;
		int getAnimationHandle(const char* name) const;
		int getAnimationHandle(const String& name) const;
		const SpriteInfo& getSprite(uint32_t handle) const;
		const AnimationInfo& getAnimation(uint32_t handle) const;
		bool SpriteValid(uint32_t handle) const;
		bool AnimationValid(uint32_t handle) const;

	};
}