#pragma once
/********************************************
*	class:	Image
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
	class LuaStack;
	class TextureCache;

	class Image
	{
	public:
		Vec2f m_texCoords[4];
		uint32_t m_textureID;
		float m_ratio;
		std::string m_name;
		size_t m_fileHash;
	};

	class ImageDefaults
	{
	public:
		uint32_t textureID;
		uint32_t width;
		uint32_t height;
	};

	LoadResult loadImage(Image& outImage, LuaStack& lua, size_t fileHash, const ImageDefaults& defaults, TextureCache& textures);
	void unloadImage(Image& image);
}
