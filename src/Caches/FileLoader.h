#pragma once
/********************************************
*	class:	FileLoader
*	usage:
********************************************/
/******* C++ headers *******/
#include <functional>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class FileLoader
	{
	public:
		bool loadFile(const char* filename);
		bool reloadFile(const char* filename);
		void unloadFile(const char* filename);

		template<typename H>
		void registerHandler(const char* ext, H& handler, bool replaceExisting = true);
		void clearHandler(const char* ext);
		
	private:
		enum class RequestType
		{
			LOAD,
			RELOAD,
			UNLOAD
		};

		typedef std::function<bool(RequestType, const char*, uint32_t)> ExtHandler;
		typedef std::pair<std::string, ExtHandler> ExtHandlerPair;
		typedef std::vector<ExtHandlerPair> ExtHandlerList;

		std::vector<std::string> m_files;
		ExtHandlerList m_handlers;

		uint32_t getIndex(const std::vector<std::string>& container, const char* item);
		uint32_t getIndex(const ExtHandlerList& container, const char* ext);
		bool invokeHandler(RequestType rt, const char* filename, uint32_t fileID);
	};


	template<typename H>
	void FileLoader::registerHandler(const char* ext, H& extHandler, bool replaceExisting)
	{
		ExtHandler newHandler = [&](RequestType rt, const char* filename, uint32_t fileID) -> bool
		{
			bool status = false;
			switch(rt)
			{
				case RequestType::LOAD:
				{
					status = extHandler.loadFile(filename, fileID);
				} break;

				case RequestType::RELOAD:
				{
					status = extHandler.reloadFile(filename, fileID);
				} break;

				case RequestType::UNLOAD:
				{
					status = extHandler.unloadFile(fileID);
				} break;

				default:
					break;
			}
			return status;
		};

		auto index = getIndex(m_handlers, ext);
		
		if(index == m_handlers.size())
		{
			m_handlers.emplace_back(ext, h);
		}
		else if(replaceExisting)
		{
			m_handlers[index].second = h;
		}
	}
}
