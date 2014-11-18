//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/PackageCache.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool PackageCache::init()
	{
		DEBUG_STATUS(true);

		DEBUG_INIT(PackageStore);
	}

	bool PackageCache::shutdown()
	{
		DEBUG_STATUS(true);

		DEBUG_SHUTDOWN(PackageStore);
	}
	
	Package& PackageCache::getPackage(const std::string& pkgName)
	{
		auto it = std::find_if(m_packages.begin(), m_packages.end(), [&](const Entry& pkg)
		{
			return pkg.first == pkgName;
		});
		if(it == m_packages.end())
		{
			m_packages.emplace_back(pkgName, Package{});
			return m_packages.back().second;
		}
		return it->second;
	}
}
