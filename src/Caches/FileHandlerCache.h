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
#include <Loaders/Defines.h>
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;
	
	typedef std::function<LoadResult(const ResourceFile&)> FileHandler;

	class FileHandlerCache
	{
	public:
		bool init(std::string successMsg, std::string failureMsg);
		bool shutdown();

		LoadResult dispatch(const ResourceFile& file);

		void registerHandler(const char* fileExtension, FileHandler handler, bool replaceExisting = true);
		void clearHandler(const char* fileExtension);

	private:
		typedef std::pair<std::string, FileHandler> Entry;
		
		std::string m_successMessage;
		std::string m_failureMessage;
		std::vector<Entry> m_handlers;

		uint32_t getIndex(const char* ext);
	};
}
