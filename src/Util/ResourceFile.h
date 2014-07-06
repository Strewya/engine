#pragma once
/********************************************
*	class:	ResourceFile
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile
	{
	public:
		ResourceFile(const char* filename);
		ResourceFile(const std::string& filename);

		const std::string& getName() const;
		const std::string& getPath() const;

		bool operator==(const ResourceFile& rhs) const;
		bool operator!=(const ResourceFile& rhs) const;

	private:
		std::string m_resourceName;
		std::string m_resourcePath;
	};

	std::ostream& operator<<(std::ostream& os, const ResourceFile& file);
}
