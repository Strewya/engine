//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ImageCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	ImageLoader::ImageLoader(TextureCache& textures)
		: m_textures(&textures)
	{}

	bool ImageLoader::load(ImageVector& images, std::vector<uint32_t>* outIDs, DataFile& file) const
	{
		auto inserter = [&](const Image& img)
		{
			auto id = findResourceByName(images, img.m_name.c_str());
			if(id == INVALID_ID)
			{
				if(outIDs != nullptr)
				{
					outIDs->emplace_back(images.size());
				}
				images.emplace_back(img);
				return true;
			}
			DEBUG_INFO("Image couldn't be loaded, name '", img.m_name, "' already exists!");
			return false;
		};
		return processLoading(images, file, inserter);
	}

	bool ImageLoader::reload(ImageVector& images, std::vector<uint32_t>* outIDs, DataFile& file) const
	{
		auto inserter = [&](const Image& img)
		{
			auto id = findResourceByName(images, img.m_name.c_str());
			if(id != INVALID_ID)
			{
				images[id] = img;
				//it's a reload, so no need to touch the outID, it's the same
				return true;
			}
			DEBUG_INFO("Image couldn't be reloaded, name '", img.m_name, "' doesn't exists!");
			return false;
		};
		return processLoading(images, file, inserter);
	}

	bool ImageLoader::unload(ImageVector& images, uint32_t id) const
	{
		return true;
	}

	bool ImageLoader::unloadAll(ImageVector& images) const
	{
		return true;
	}

	bool ImageLoader::processLoading(ImageVector& images, DataFile& file, const Inserter& inserter) const
	{
		assert(m_textures != nullptr);
		bool status = false;
		auto textureName = file.getString("texture", "");
		uint32_t textureID = INVALID_ID;
		if(m_textures->loadFromFile(false, &textureID, textureName))
		{
			Vec2 td = m_textures->getTextureDimensions(textureID);
			auto tw = td.x;
			auto th = td.y;
			auto defaultWidth = file.getInt("defaultWidth", 0);
			auto defaultHeight = file.getInt("defaultHeight", 0);
			auto imageCount = file.getListSize("list");
			images.reserve(images.size() + imageCount);
			if(file.getList("list"))
			{
				status = true;
				for(uint32_t i = 0; i < imageCount; ++i)
				{
					if(file.getList(i + 1))
					{
						Image img;
						img.m_textureID = textureID;
						if(parseImage(img, file, tw, th, defaultWidth, defaultHeight))
						{
							status &= inserter(img);
						}
						file.popList();
					}
					else
					{
						DEBUG_INFO("Element at index '", i + 1, "' is either nil or not an image description!");
					}
				}
				file.popList();
			}
			else
			{
				DEBUG_INFO("The image data contains no list of images!!");
			}
		}
		else
		{
			DEBUG_INFO("Invalid texture name ", textureName);
		}
		return status;
	}

	bool ImageLoader::parseImage(Image& img, DataFile& file, float textureW, float textureH, uint32_t defaultW, uint32_t defaultH) const
	{
		bool parseOK = false;
		img.m_name.assign(file.getString("name", ""));
		auto imgWidth = file.getInt("width", defaultW);
		auto imgHeight = file.getInt("height", defaultH);
		auto pos = file.getVec2("pos", Vec2(-1, -1));
		if(!img.m_name.empty() && imgWidth > 0 && imgHeight > 0 && pos.x >= 0 && pos.y >= 0)
		{
			float w = static_cast<float>(imgWidth);
			float h = static_cast<float>(imgHeight);

			//image is valid, fill and add it
			img.m_ratio = w / h;
			Vec2 wh = pos + Vec2(w, h);

			/* the vertices are in the following order:
			0--1
			|  |
			3--2
			*/

			img.m_texCoords[0].x = pos.x / textureW;
			img.m_texCoords[0].y = pos.y / textureH;

			img.m_texCoords[2].x = wh.x / textureW;
			img.m_texCoords[2].y = wh.y / textureH;

			img.m_texCoords[1].x = img.m_texCoords[2].x;
			img.m_texCoords[1].y = img.m_texCoords[0].y;

			img.m_texCoords[3].x = img.m_texCoords[0].x;
			img.m_texCoords[3].y = img.m_texCoords[2].y;

			parseOK = true;
		}
		else
		{
			DEBUG_INFO("Image is invalid: ", img.m_name, ",", imgWidth, ",", imgHeight, ",", pos);
		}
		return parseOK;
	}
}
