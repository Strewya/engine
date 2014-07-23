//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FileHandlerCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	std::string getExtension(const std::string& file)
	{
		auto pos = file.find_last_of('.');
		return file.substr(pos + 1);
	}

	bool FileHandlerCache::init()
	{
		bool status = true;

		DEBUG_INIT(FileHandlerCache);
		return status;
	}
	
	bool FileHandlerCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(FileHandlerCache);
		return status;
	}

	void FileHandlerCache::registerHandler(const char* fileExtension, FileHandler handler, bool replaceExisting)
	{
		uint32_t id = getIndex(fileExtension);
		if(id == m_handlers.size())
		{
			m_handlers.emplace_back(std::make_pair(fileExtension, handler));
		}
		else if(replaceExisting)
		{
			m_handlers[id].second = handler;
		}
	}

	void FileHandlerCache::clearHandler(const char* extension)
	{
		auto index = getIndex(extension);
		if(index < m_handlers.size())
		{
			std::swap(m_handlers[index], m_handlers.back());
			m_handlers.pop_back();
		}
	}

	bool FileHandlerCache::dispatch(const ResourceFile& file)
	{
		uint32_t index = getIndex(getExtension(file.getName()).c_str());
		if(index != m_handlers.size())
		{
			return m_handlers[index].second(file);
		}
		return false;
	}

	uint32_t FileHandlerCache::getIndex(const char* ext)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_handlers), end(m_handlers),
			[&](const Entry& handler)
		{
			return handler.first == ext;
		});
		return std::distance(begin(m_handlers), it);
	}
}
