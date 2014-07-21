//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/PackageManager.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/DataFile.h>
/******* end headers *******/

namespace Core
{
	bool PackageManager::init()
	{
		return true;
	}

	bool PackageManager::shutdown()
	{
		return true;
	}

	void PackageManager::parse(DataFile& file)
	{
		//parse a file that contains the package names and contents
		
	}

	void PackageManager::loadPackage(const char* name)
	{

	}

	void PackageManager::releasePackage(const char* name)
	{

	}
}
