//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Loaders/PackageLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/FileHandlerCache.h>
#include <Caches/PackageCache.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool PackageLoader::init(PackageCache& packages, FileHandlerCache& loadHandlers, FileHandlerCache& unloadHandlers)
	{
		bool status = true;

		m_packages = &packages;
		m_loadHandlers = &loadHandlers;
		m_unloadHandlers = &unloadHandlers;

		DEBUG_INIT(PackageLoader);
		return status;
	}

	bool PackageLoader::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(PackageLoader);
		return status;
	}

	bool PackageLoader::loadPackage(const std::string& pkgName)
	{
		auto& pkg = m_packages->getPackage(pkgName);
		const auto& fileList = pkg.getFiles();
		bool status = true;
		for(const auto& file : fileList)
		{
			status = m_loadHandlers->dispatch(file) && status;
		}
		return status;
	}

	bool PackageLoader::releasePackage(const std::string& pkgName)
	{
		auto& pkg = m_packages->getPackage(pkgName);
		const auto& fileList = pkg.getFiles();
		bool status = true;
		for(const auto& file : fileList)
		{
			status = m_unloadHandlers->dispatch(file) && status;
		}
		return status;
	}
}
