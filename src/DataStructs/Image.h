#pragma once
/********************************************
*	class:	Image
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
	class TextureCache;

	class Image
	{
	public:
		uint32_t m_textureID;
		float m_ratio;
		Vec2 m_texCoords[4];
		std::string m_name;
	};

	class ImageDefaults
	{
	public:
		uint32_t textureID;
		uint32_t width;
		uint32_t height;
	};

	bool parseImage(Image& outImage, DataFile& file, const ImageDefaults& defaults, TextureCache& textures);
}
