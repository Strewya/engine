//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Image.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Scripting/LuaStack.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	LoadResult loadImage(Image& outImage, LuaStack& lua, size_t fileHash, const ImageDefaults& defaults, TextureCache& textures)
	{
		if( !lua.isString(-2) || !lua.isTable(-1) )
		{
			return{LoadResultFlag::Fail, "Invalid image format"};
		}

		outImage.m_name = lua.toString(-2);
		outImage.m_textureID = defaults.textureID;

		Vec2 pos(-1, -1);
		lua.pull("pos");
		if( lua.isTable() )
		{
			pos.x = getFloat(lua, "x", pos.x);
			if( pos.x < 0 ) pos.x = getFloat(lua, 1, pos.x);
			pos.y = getFloat(lua, "y", pos.y);
			if( pos.y < 0 ) pos.y = getFloat(lua, 2, pos.y);
		}
		lua.pop();
		auto imgWidth = getInt(lua, "width", defaults.width);
		auto imgHeight = getInt(lua, "height", defaults.height);

		if( outImage.m_name.empty() || pos.x < 0 || pos.y < 0 || imgWidth == 0 || imgHeight == 0 )
		{
			return{LoadResultFlag::Fail, "Invalid image format"};
		}

		auto w = static_cast<float>(imgWidth);
		auto h = static_cast<float>(imgHeight);
		auto wh = pos + Vec2(w, h);
		const auto* texture = textures.getResource(outImage.m_textureID);
		//i'm assuming texture is not nullptr, because ImageCache takes care of loading the dependency
		auto& dimensions = texture->m_dimensions;

		/* the vertices are in the following order:
		0--1
		|  |
		3--2
		*/

		outImage.m_ratio = w / h;

		outImage.m_texCoords[0].x = pos.x / dimensions.x;
		outImage.m_texCoords[0].y = pos.y / dimensions.y;

		outImage.m_texCoords[2].x = wh.x / dimensions.x;
		outImage.m_texCoords[2].y = wh.y / dimensions.y;

		outImage.m_texCoords[1].x = outImage.m_texCoords[2].x;
		outImage.m_texCoords[1].y = outImage.m_texCoords[0].y;

		outImage.m_texCoords[3].x = outImage.m_texCoords[0].x;
		outImage.m_texCoords[3].y = outImage.m_texCoords[2].y;

		outImage.m_fileHash = fileHash;
		return{LoadResultFlag::Success};
	}

	void unloadImage(Image& image)
	{
		image.m_name.clear();
		image.m_ratio = 0;
		for( uint32_t i = 0; i < 4;++i )
		{
			image.m_texCoords[i].set(-1, -1);
		}
		image.m_textureID = INVALID_ID;
		image.m_fileHash = 0;
	}
}
