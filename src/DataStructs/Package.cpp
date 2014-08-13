//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Package.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	void Package::addFile(ResourceFile file)
	{
		m_files.emplace_back(file);
	}

	PackageFileList Package::getFiles() const
	{
		return m_files;
	}

	void Package::removeDuplicateFiles()
	{
		std::sort(m_files.begin(), m_files.end(), [](const ResourceFile& l, const ResourceFile& r)
		{
			return l.getName() < r.getName();
		});
		auto it = std::unique(m_files.begin(), m_files.end());
		m_files.erase(it, m_files.end());
		m_files.shrink_to_fit();
	}
}
