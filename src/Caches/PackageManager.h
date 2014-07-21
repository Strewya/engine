#pragma once
/********************************************
*	class:	PackageManager
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class DataFile;

	class PackageManager
	{
	public:
		bool init();
		bool shutdown();

		void parse(DataFile& file);

		void loadPackage(const char* pkgName);
		void releasePackage(const char* pkgName);

	private:
		struct Package
		{
			std::string name;
			std::vector<std::string> fileList;
			bool loaded;
		};

		std::vector<Package> m_packages;
	};
}
