#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Modules/Graphics/Service/AnimationInfo.h>
#include <Modules/Graphics/Service/SpriteInfo.h>
#include <Modules/Graphics/Service/TextureData.h>
	/*** end header inclusion ***/

namespace Graphics
{
	class Spritesheet
	{
	private:
		std::string _sheetName;
		std::string _textureName;
		uint32_t _textureHandle;
		std::deque<SpriteInfo> _sprites;
		std::deque<AnimationInfo> _animations;

	public:
		Spritesheet();
		Spritesheet(const char* sheetName);
		Spritesheet(const std::string& sheetName);
		~Spritesheet();

		uint32_t getTextureHandle() const;
		const std::string& getTextureName() const;
		const std::string& getSpritesheetName() const;

		void setTexture(uint32_t textureHandle);
		void setTextureName(const char* name);
		void setTextureName(const std::string& name);
		void setSpritesheetName(const char* name);
		void setSpritesheetName(const std::string& name);

		uint32_t Insert(const SpriteInfo& sprite);
		uint32_t Insert(const AnimationInfo& animation);

		int getSpriteHandle(const char* name) const;
		int getSpriteHandle(const std::string& name) const;
		int getAnimationHandle(const char* name) const;
		int getAnimationHandle(const std::string& name) const;
		const SpriteInfo& getSprite(uint32_t handle) const;
		const AnimationInfo& getAnimation(uint32_t handle) const;
		bool SpriteValid(uint32_t handle) const;
		bool AnimationValid(uint32_t handle) const;

	};
}