//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FontCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/DataFile.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool FontCache::init(TextureCache& textures)
	{
		bool status = true;

		m_textures = &textures;

		DEBUG_INIT(FontCache);
		return status;
	}

	bool FontCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(FontCache);
		return status;
	}

	uint32_t FontCache::getFontID(const char* name) const
	{
		auto it = std::find_if(m_fonts.begin(), m_fonts.end(), [&](const Font& f)
		{
			return f.m_name == name;
		});
		if(it != m_fonts.end())
		{
			return std::distance(m_fonts.begin(), it) + 1;
		}
		return 0;
	}

	const Font& FontCache::getFont(uint32_t id) const
	{
		--id;
		assert(id < m_fonts.size());
		return m_fonts[id];
	}

	bool FontCache::load(const ResourceFile& file, DataFile& dataFile)
	{
		auto it = std::find_if(m_fonts.begin(), m_fonts.end(), [&](const Font& f)
		{
			return f.m_name == file.getName();
		});
		if(it == m_fonts.end())
		{
			if(dataFile.open(file.getPath()))
			{
				m_fonts.emplace_back();
				parseFont(m_fonts.back(), dataFile, *m_textures);
			}
			
		}
	}

	bool FontCache::reload(const ResourceFile& file, DataFile& dataFile)
	{

	}

	bool FontCache::unload(const ResourceFile& file)
	{

	}
}
