//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Loaders/PackageLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/FileHandlerCache.h>
#include <Caches/PackageCache.h>
#include <Loaders/Defines.h>
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
		auto fileList = pkg.getFiles();
		bool allLoaded = true;
		while( !fileList.empty() )
		{
			auto& file = fileList.back();
			auto res = m_loadHandlers->dispatch(file);
			if( res.flag == LoadResultFlag::DependencyMissing )
			{
				pkg.addFile(res.info);
				fileList.emplace_back(res.info);
			}
			else if( res.flag == LoadResultFlag::Success )
			{
				fileList.pop_back();
			}
			else
			{
				allLoaded = false;
			}
		}
		return allLoaded;
	}

	bool PackageLoader::releasePackage(const std::string& pkgName)
	{
		auto& pkg = m_packages->getPackage(pkgName);
		auto fileList = pkg.getFiles();
		bool allUnloaded = true;
		while( !fileList.empty() )
		{
			auto& file = fileList.back();
			auto res = m_unloadHandlers->dispatch(file);
			if( res.flag == LoadResultFlag::Success )
			{
				fileList.pop_back();
			}
			else
			{
				allUnloaded = false;
			}
		}
		return allUnloaded;
		
	}
}
