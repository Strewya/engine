//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ImageCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool ImageCache::init()
	{
		bool status = true;

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
		assert(id < m_images.size());
		return m_images[id];
	}

	uint32_t ImageCache::getImageID(const char* name) const
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_images), end(m_images), [&](const Image& i)
		{
			return i.m_name == name;
		});
		uint32_t index = -1;
		if(it != end(m_images))
		{
			index = std::distance(begin(m_images), it);
		}
		return index;
	}

	bool ImageCache::addImage(const Image& image, uint32_t* outIndex)
	{
		auto index = getImageID(image.m_name.c_str());
		bool added = false;
		if(index == -1)
		{
			m_images.emplace_back(image);
			added = true;
			if(outIndex != nullptr)
			{
				*outIndex = m_images.size() - 1;
			}
		}
		return added;
	}
}
