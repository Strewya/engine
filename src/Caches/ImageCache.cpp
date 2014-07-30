//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ImageCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/TextureCache.h>
#include <Util/DataFile.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool ImageCache::init(TextureCache& textures)
	{
		bool status = true;

		m_textures = &textures;

		DEBUG_INIT(ImageCache);
		return status;
	}

	bool ImageCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(ImageCache);
		return status;
	}

	const Image& ImageCache::getImage(uint32_t id) const
	{
		
	}

	uint32_t ImageCache::getImageID(const char* name) const
	{
		
	}

	bool ImageCache::load(const ResourceFile& file)
	{
		bool success = false;
		
		return success;
	}
}
