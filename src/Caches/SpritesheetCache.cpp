//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/SpritesheetCache.h>
/******* C++ headers *******/
#include <cassert>
/******* extra headers *******/
#include <Caches/AnimationCache.h>
#include <Caches/ImageCache.h>
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	static bool parseSheetData(Spritesheet& sheet, bool reload, DataFile& file, AnimationCache& animations, ImageCache& images, TextureCache& textures);
	static bool parseImageData(std::vector<uint32_t>& indices, bool reload, DataFile& file, ImageCache& images, TextureCache& textures);
	static bool parseAnimationData(std::vector<uint32_t>& indices, bool reload, DataFile& file, AnimationCache& animations, ImageCache& images);

	bool SpritesheetCache::init(AnimationCache& animations, ImageCache& images, TextureCache& textures)
	{
		bool status = true;

		m_animations = &animations;
		m_images = &images;
		m_textures = &textures;

		DEBUG_INIT(SpritesheetCache);
		return status;
	}

	bool SpritesheetCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(SpritesheetCache);
		return status;
	}

	bool SpritesheetCache::loadFromFile(DataFile& file, bool reload)
	{
		bool status = false;
		//get sheet id
		auto id = getSpritesheetID(file.getFilename().c_str());

		//reload && id==-1 : error
		if(reload && id == -1)
		{
			DEBUG_INFO("The spritesheet ", file.getFilename(), " cannot be reloaded, it does not exist!");
		}
		//reload && id!=-1 : clean sheet and fill
		else if(reload && id != -1)
		{
			//we need to destroy all images and animations that were loaded by this sheet previously (texture too?)
			//do destroy...
			status = parseSheetData(m_sheets[id], reload, file, *m_animations, *m_images, *m_textures);
		}
		//!reload && id==-1 : create sheet and fill
		else if(!reload && id == -1)
		{
			id = m_sheets.size();
			m_sheets.emplace_back();
			status = parseSheetData(m_sheets[id], reload, file, *m_animations, *m_images, *m_textures);
		}
		//!reload && id!=-1 : error
		else if(!reload && id != -1)
		{
			DEBUG_INFO("The spritesheet ", file.getFilename(), " cannot be loaded, another with the same name exists!");
		}
		
		return status;
	}
	
	uint32_t SpritesheetCache::getSpritesheetID(const char* name) const
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_sheets), end(m_sheets), [&](const Spritesheet& sheet)
		{
			return name == sheet.m_name;
		});

		if(it != end(m_sheets))
		{
			return std::distance(begin(m_sheets), it);
		}
		return -1;
	}

	const Spritesheet& SpritesheetCache::getSpritesheet(uint32_t id) const
	{
		assert(id < m_sheets.size());
		return m_sheets[id];
	}

	bool parseSheetData(Spritesheet& sheet, bool reload, DataFile& file, AnimationCache& animations, ImageCache& images, TextureCache& textures)
	{
		bool success = false;
		sheet.m_name.assign(file.getFilename());
		success = parseImageData(sheet.m_images, reload, file, images, textures) &&
			parseAnimationData(sheet.m_animations, reload, file, animations, images);
		return success;
	}

	bool parseImageData(std::vector<uint32_t>& indices, bool reload, DataFile& file, ImageCache& images, TextureCache& textures)
	{
		bool success = false;
		if(file.getList("imageData"))
		{
			auto textureName = file.getString("texture", "");
			if(!textureName.empty())
			{
				auto textureID = textures.getTextureID(textureName.c_str());
				if(textureID != -1)
				{
					Vec2 td = textures.getTextureDimensions(textureID);
					float tw = td.x;
					float th = td.y;
					auto defaultWidth = file.getInt("defaultWidth", 0);
					auto defaultHeight = file.getInt("defaultHeight", 0);
					auto imageCount = file.getListSize("list");
					indices.resize(imageCount);
					if(file.getList("list"))
					{
						for(uint32_t i = 0; i < imageCount; ++i)
						{
							if(file.getList(i + 1))
							{
								Image img;
								img.m_name.assign(file.getString("name", ""));
								img.m_textureID = textureID;
								auto imgWidth = file.getInt("width", defaultWidth);
								auto imgHeight = file.getInt("height", defaultHeight);
								auto pos = file.getVec2("pos", Vec2(-1, -1));
								if(!img.m_name.empty() && imgWidth > 0 && imgHeight > 0 && pos.x >= 0 && pos.y >= 0)
								{
									float w = static_cast<float>(imgWidth);
									float h = static_cast<float>(imgHeight);

									//image is valid, fill and add it
									img.m_ratio = w / h;
									Vec2 wh = pos + Vec2(w, h);

									img.m_texCoords[0].x = pos.x / tw;
									img.m_texCoords[0].y = pos.y / th;
									
									img.m_texCoords[2].x = wh.x / tw;
									img.m_texCoords[2].y = wh.y / th;

									img.m_texCoords[1].x = img.m_texCoords[2].x;
									img.m_texCoords[1].y = img.m_texCoords[0].y;

									img.m_texCoords[3].x = img.m_texCoords[0].x;
									img.m_texCoords[3].y = img.m_texCoords[2].y;

									if(!images.addImage(img, reload, &indices[i]))
									{
										DEBUG_INFO("Failed to add image with name ", img.m_name, ", skipping it!");
									}
								}
								else
								{
									DEBUG_INFO("Image in ", file.getFilename(), " spritesheet is invalid: ", img.m_name, ",", imgWidth, ",", imgHeight, ",", pos);
								}
								file.popList();
							}
							else
							{
								DEBUG_INFO("Element at index '", i + 1, "' is either nil or not a list!");
							}
						}
						file.popList();
					}
					else
					{
						DEBUG_INFO("The image data contains no list of images!!");
					}
					success = !indices.empty();
				}
				else
				{
					DEBUG_INFO("The texture ", textureName, " doesn't exist!");
				}
			}
			else
			{
				DEBUG_INFO("Texture name is missing in file ", file.getFilename());
			}
			file.popList();
		}
		else
		{
			DEBUG_INFO("Config file '", file.getFilename(), "' contains no images!");
		}
		return success;
	}

	bool parseAnimationData(std::vector<uint32_t>& indices, bool reload, DataFile& file, AnimationCache& animations, ImageCache& images)
	{
		bool success = false;
		if(file.getList("animationData"))
		{
			auto defaultDuration = file.getFloat("defaultDuration", 0);
			auto defaultRepeat = file.getBool("defaultRepeat", false);
			auto animCount = file.getListSize("list");
			indices.resize(animCount);
			if(file.getList("list"))
			{
				for(uint32_t i = 0; i < animCount; ++i)
				{
					if(file.getList(i + 1))
					{
						Animation anim;
						anim.m_name.assign(file.getString("name", ""));
						anim.m_defaultDuration = Time::secondsToMilis(file.getFloat("duration", defaultDuration));
						anim.m_defaultRepeat = file.getBool("loop", defaultRepeat);
						auto imageCnt = file.getListSize("images");
						anim.m_sequence.resize(imageCnt);
						if(!anim.m_name.empty() && anim.m_defaultDuration > 0 && imageCnt > 0)
						{
							//anim valid, parse images
							if(file.getList("images"))
							{
								for(uint32_t j = 0; j < imageCnt; ++j)
								{
									auto imgName = file.getString(i + 1, "");
									auto imgID = images.getImageID(imgName.c_str());
									if(imgID != -1)
									{
										anim.m_sequence[j] = imgID;
									}
								}
								file.popList();
							}
							else
							{
								DEBUG_INFO("Could not get image list, either not exists of not a list!");
							}

							if(!animations.addAnimations(anim, reload, &indices[i]))
							{
								DEBUG_INFO("Failed to add animation '", anim.m_name, "', skipping it!");
							}
						}
						else
						{
							DEBUG_INFO("Animation not valid!");
						}
						file.popList();
					}
					else
					{
						DEBUG_INFO("Element at index '", i + 1, "' is either nil or not a list!");
					}
				}
				file.popList();
			}
			else
			{
				DEBUG_INFO("The animation data contains no animation list!");
			}
			file.popList();
			success = !indices.empty();
		}
		else
		{
			DEBUG_INFO("Config file '",file.getFilename(),"' contains no animations.");
			success = true;
		}

		return success;
	}
}