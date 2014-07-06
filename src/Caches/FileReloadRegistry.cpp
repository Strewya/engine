//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/FileReloadRegistry.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/ResourceFile.h>
/******* end headers *******/

namespace Core
{
	std::string getExtension(const std::string& file)
	{
		auto pos = file.find_last_of('.');
		return file.substr(pos + 1);
	}

	uint32_t FileReloadRegistry::getIndex(const char* ext)
	{
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_handlers), end(m_handlers),
			[&](const ExtHandlerPair& handler)
		{
			return handler.first == ext;
		});
		return std::distance(begin(m_handlers), it);
	}

	void FileReloadRegistry::registerHandler(const char* ext, const ExtHandler& extHandler, bool replaceExisting)
	{
		uint32_t id = getIndex(ext);
		if(id == m_handlers.size())
		{
			m_handlers.emplace_back(std::make_pair(ext, extHandler));
		}
		else if(replaceExisting)
		{
			m_handlers[id].second = extHandler;
		}
	}

	void FileReloadRegistry::clearHandler(const char* extension)
	{
		auto index = getIndex(extension);
		if(index < m_handlers.size())
		{
			m_handlers.erase(m_handlers.begin() + index);
		}
	}

	bool FileReloadRegistry::onFileModified(const ResourceFile& file)
	{
		uint32_t index = getIndex(getExtension(file.getName()).c_str());
		if(index != m_handlers.size())
		{
			return m_handlers[index].second(file);
		}
		return false;
	}
}
