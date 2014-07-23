#pragma once
/********************************************
*	class:	PackageStore
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Package.h>
/******* end header inclusion *******/

namespace Core
{
	class PackageCache
	{
	public:
		bool init();
		bool shutdown();

		Package& getPackage(const std::string& pkgName);
		
	private:
		using Entry = std::pair < std::string, Package > ;
		std::vector<Entry> m_packages;
	};
}
