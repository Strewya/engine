#pragma once
/********************************************
*	class:	Package
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/ResourceFile.h>
/******* end header inclusion *******/

namespace Core
{
	using PackageFileList = std::vector < ResourceFile >;

	class Package
	{
	public:
		void addFile(ResourceFile file);
		void removeDuplicateFiles();
		
		const PackageFileList& getFiles() const;

	private:
		PackageFileList m_files;
	};
}
