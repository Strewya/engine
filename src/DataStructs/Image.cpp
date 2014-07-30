//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Image.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool parseImage(Image& outImage, DataFile& file, const ImageDefaults& defaults, const TextureCache& textures)
	{
		auto success = false;
		auto name = file.getString(-2, "");
		auto pos = file.getVec2("pos", Vec2(-1, -1));
		auto imgWidth = file.getInt("width", defaults.width);
		auto imgHeight = file.getInt("height", defaults.height);
		auto texture = file.getString("texture", "");
		auto textureID = texture.empty() ? defaults.textureID : textures.getTextureID(texture.c_str());

		if( !name.empty() && pos.x >= 0 && pos.y >= 0 && imgWidth > 0 && imgHeight > 0 && textureID != 0 )
		{
			auto w = static_cast<float>(imgWidth);
			auto h = static_cast<float>(imgHeight);
			auto wh = pos + Vec2(w, h);
			auto dimensions = textures.getTextureDimensions(textureID);

			/* the vertices are in the following order:
			0--1
			|  |
			3--2
			*/

			outImage.m_name = name;
			outImage.m_textureID = textureID;
			outImage.m_ratio = w / h;

			outImage.m_texCoords[0].x = pos.x / dimensions.x;
			outImage.m_texCoords[0].y = pos.y / dimensions.y;

			outImage.m_texCoords[2].x = wh.x / dimensions.x;
			outImage.m_texCoords[2].y = wh.y / dimensions.y;

			outImage.m_texCoords[1].x = outImage.m_texCoords[2].x;
			outImage.m_texCoords[1].y = outImage.m_texCoords[0].y;

			outImage.m_texCoords[3].x = outImage.m_texCoords[0].x;
			outImage.m_texCoords[3].y = outImage.m_texCoords[2].y;

			success = true;
		}
		else
		{
			DEBUG_INFO("Image in ", file.getFilename(), " spritesheet is invalid: ", name, ",", imgWidth, ",", imgHeight, ",", pos);
		}
		return success;
	}
}
