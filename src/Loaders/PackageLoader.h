#pragma once
/********************************************
*	class:	PackageLoader
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class PackageCache;
	class FileHandlerCache;

	class PackageLoader
	{
	public:
		bool init(PackageCache& packages, FileHandlerCache& loadHandlers, FileHandlerCache& unloadHandlers);
		bool shutdown();

		bool loadPackage(const std::string& pkgName);
		bool releasePackage(const std::string& pkgName);

	private:
		PackageCache* m_packages;
		FileHandlerCache* m_loadHandlers;
		FileHandlerCache* m_unloadHandlers;
	};
}
