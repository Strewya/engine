#pragma once
/********************************************
*	class:	FileReloadRegistry
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

	class FileReloadRegistry
	{
	public:
		typedef std::function<bool(const ResourceFile&)> ExtHandler;

		bool onFileModified(const ResourceFile& file);

		void registerHandler(const char* ext, const ExtHandler& handler, bool replaceExisting = true);

		void clearHandler(const char* ext);

	private:
		typedef std::pair<std::string, ExtHandler> ExtHandlerPair;
		typedef std::vector<ExtHandlerPair> ExtHandlerList;

		ExtHandlerList m_handlers;

		uint32_t getIndex(const char* ext);
	};
}
