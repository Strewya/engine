#pragma once
/********************************************
*	class:	FileHandlerCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;
	
	typedef std::function<bool(const ResourceFile&)> FileHandler;

	class FileHandlerCache
	{
	public:
		bool init();
		bool shutdown();

		bool dispatch(const ResourceFile& file);

		void registerHandler(const char* fileExtension, FileHandler handler, bool replaceExisting = true);
		void clearHandler(const char* fileExtension);

	private:
		typedef std::pair<std::string, FileHandler> Entry;
		
		std::vector<Entry> m_handlers;

		uint32_t getIndex(const char* ext);
	};
}
