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

	bool FileHandlerCache::init(std::string successMsg, std::string failureMsg)
	{
		bool status = true;

		m_successMessage = std::move(successMsg);
		m_failureMessage = std::move(failureMsg);

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
			m_handlers.emplace_back(fileExtension, handler);
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

	LoadResult FileHandlerCache::dispatch(const ResourceFile& file)
	{
		LoadResult res;
		uint32_t index = getIndex(getExtension(file.getName()).c_str());
		if(index != m_handlers.size())
		{
			res = m_handlers[index].second(file);
			std::string* pstr = &m_failureMessage;
			if( res )
			{
				pstr = &m_successMessage;
			}
			DEBUG_INFO(*pstr, " ", file.getName(), " ", res);
		}
		else
		{
			DEBUG_INFO("No handler for file ", file.getName());
		}
		return res;
	}

	uint32_t FileHandlerCache::getIndex(const char* ext)
	{
		return filterFind(m_handlers,
			[&](const Entry& handler)
		{
			return handler.first == ext;
		});
	}
}
