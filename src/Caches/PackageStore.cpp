//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/PackageStore.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Package& PackageStore::getPackage(const std::string& pkgName)
	{
		auto it = std::find_if(m_packages.begin(), m_packages.end(), [&](const Entry& pkg)
		{
			return pkg.first == pkgName;
		});
		if(it == m_packages.end())
		{
			m_packages.emplace_back(std::make_pair(pkgName, Package{}));
			return m_packages.back().second;
		}
		return it->second;
	}
}
