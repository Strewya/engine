//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/ResourceFile.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	ResourceFile::ResourceFile(const char* filename)
		: m_resourceName(filename)
	{
		replace(replace(m_resourceName, "../", ""), "resources/", "");
		m_resourcePath = "../resources/" + m_resourceName;
	}

	ResourceFile::ResourceFile(const std::string& filename)
		: ResourceFile(filename.c_str())
	{
	}

	const std::string& ResourceFile::getName() const
	{
		return m_resourceName;
	}

	const std::string& ResourceFile::getPath() const
	{
		return m_resourcePath;
	}

	bool ResourceFile::operator==(const ResourceFile& rhs) const
	{
		return (m_resourcePath == rhs.m_resourcePath);
	}

	bool ResourceFile::operator!=(const ResourceFile& rhs) const
	{
		return !operator==(rhs);
	}

	std::ostream& operator<<(std::ostream& os, const ResourceFile& file)
	{
		os << file.getPath();
		return os;
	}
}
