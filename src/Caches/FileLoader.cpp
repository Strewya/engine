//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FileLoader.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	std::string getExtension(const std::string& file)
	{
		auto pos = file.find_last_of('.');
		return file.substr(pos + 1);
	}

	bool FileLoader::invokeHandler(RequestType rt, const char* filename, uint32_t fileID)
	{
		auto handlerIndex = getIndex(m_handlers, getExtension(filename).c_str());
		if(handlerIndex < m_handlers.size())
		{
			return m_handlers[handlerIndex].second(rt, filename, fileID);
		}
		return false;
	}

	bool FileLoader::loadFile(const char* filename)
	{
		assert(filename && strlen(filename) > 0);
		bool status = false;
		auto fileIndex = getIndex(m_files, filename);
		if(fileIndex == m_files.size())
		{
			fileIndex = m_files.size();
			m_files.emplace_back(filename);
			//now we need to invoke the load for the cache...
			status = invokeHandler(RequestType::LOAD, filename, fileIndex);
		}
		return status;
	}

	bool FileLoader::reloadFile(const char* filename)
	{
		assert(filename && strlen(filename) > 0);
		bool status = false;
		auto fileIndex = getIndex(m_files, filename);
		if(fileIndex != m_files.size())
		{
			//invoke handler with RELOAD request type
			status = invokeHandler(RequestType::RELOAD, filename, fileIndex);
		}
		return status;
	}

	bool FileLoader::unloadFile(const char* filename)
	{
		assert(filename && strlen(filename) > 0);
		bool status = false;
		auto fileIndex = getIndex(m_files, filename);
		if(fileIndex != m_files.size())
		{
			m_files[fileIndex].clear();
			//now we need to invoke the unload for the cache...
			status = invokeHandler(RequestType::UNLOAD, filename, fileIndex);
		}
		return status;
	}

	uint32_t FileLoader::getIndex(const std::vector<std::string>& container, const char* item)
	{
		using std::begin; using std::end;
		auto it = std::find(begin(container), end(container), item);
		return std::distance(begin(container), it);
	}

	uint32_t FileLoader::getIndex(const ExtHandlerList& container, const char* ext)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(container), end(container),
			[&](const ExtHandlerPair& handler)
		{
			return handler.first == ext;
		});
		return std::distance(begin(container), it);
	}
}
