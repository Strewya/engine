//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ImageCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/FileLoader.h>
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	ImageLoader::ImageLoader(FileLoader& fileLoader, TextureCache& textures)
		: m_fileLoader(&fileLoader), m_textures(&textures)
	{
	}

	bool ImageLoader::load(ImageData& images, DataFile& file, uint32_t fileID) const
	{
		auto loadFilter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(images, name);
			if(id == INVALID_ID)
			{
				return images.create();
			}
			DEBUG_INFO("Image '", name, "' already loaded, skipping...");
			return INVALID_ID;
		};
		return processLoading(images, file, fileID, loadFilter);
	}

	bool ImageLoader::reload(ImageData& images, DataFile& file, uint32_t fileID) const
	{
		auto loadFilter = [&](const char* name) -> uint32_t
		{
			auto id = findResourceByName(images, name);
			if(id == INVALID_ID)
			{
				id = images.create();
			}
			else
			{
				unloadOne(images, id);
			}
			return id;
		};
		return processLoading(images, file, fileID, loadFilter);
	}

	void ImageLoader::unloadOne(ImageData& images, uint32_t id) const
	{
		auto& img = images.get(id);
		img.m_name.clear();
		img.m_fileID = INVALID_ID;
		img.m_ratio = 0;
		img.m_textureID = INVALID_ID;
		for(auto i = 0; i < 4; ++i)
		{
			img.m_texCoords[i].set(0, 0);
		}
	}

	void ImageLoader::unloadAll(ImageData& images) const
	{
		auto filter = [](const Image& i) { return true; };

		for(auto id = images.getID(filter); id != INVALID_ID; id = images.getID(filter))
		{
			unloadOne(images, id);
		}
	}

	void ImageLoader::unloadFile(ImageData& images, uint32_t fileID) const
	{
		for(auto id = findResourceByFileID(images, fileID); id != INVALID_ID; id = findResourceByFileID(images, fileID))
		{
			unloadOne(images, id);
			images.remove(id);
		}
	}

	bool ImageLoader::processLoading(ImageData& images, DataFile& file, uint32_t fileID, const Filter& filter) const
	{
		assert(m_textures != nullptr);
		bool status = false;
		auto textureName = file.getString("texture", "");
		if(m_fileLoader->loadFile(textureName.c_str()))
		{
			auto defaultWidth = file.getInt("defaultWidth", 0);
			auto defaultHeight = file.getInt("defaultHeight", 0);
			auto imageCount = file.getListSize("list");
			if(file.getList("list"))
			{
				status = true;
				for(uint32_t i = 0; i < imageCount; ++i)
				{
					if(file.getList(i + 1))
					{
						auto imgID = filter(file.getString("name", "").c_str());
						if(imgID != INVALID_ID)
						{
							auto success = parseImage(images.get(imgID), file, fileID, textureName.c_str(), defaultWidth, defaultHeight);
							if(!success)
							{
								images.remove(imgID);
							}
							status = status && success;
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

	bool ImageLoader::parseImage(Image& img, DataFile& file, uint32_t fileID, const char* textureName, uint32_t defaultW, uint32_t defaultH) const
	{
		bool parseOK = false;
		auto imgName = file.getString("name", "");
		auto imgWidth = file.getInt("width", defaultW);
		auto imgHeight = file.getInt("height", defaultH);
		auto pos = file.getVec2("pos", Vec2(-1, -1));
		if(!imgName.empty() && imgWidth > 0 && imgHeight > 0 && pos.x >= 0 && pos.y >= 0)
		{
			float w = static_cast<float>(imgWidth);
			float h = static_cast<float>(imgHeight);
			auto textureID = m_textures->getResourceID(textureName);
			auto textureDim = m_textures->getTextureDimensions(textureID);
			auto textureDimensions = m_textures->getTextureDimensions(textureID);

			//image is valid, fill and add it
			img.m_name = imgName;
			img.m_fileID = fileID; 
			img.m_ratio = w / h;
			img.m_textureID = textureID;
			
			/* the vertices are in the following order:
			0--1
			|  |
			3--2
			*/
			Vec2 wh = pos + Vec2(w, h);
			img.m_texCoords[0].x = pos.x / textureDimensions.x;
			img.m_texCoords[0].y = pos.y / textureDimensions.y;

			img.m_texCoords[2].x = wh.x / textureDimensions.x;
			img.m_texCoords[2].y = wh.y / textureDimensions.y;

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
