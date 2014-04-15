//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Caches/ScriptCache.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/ScriptingSystem.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool ScriptCache::init(ScriptingSystem& scripting)
	{
		bool status = true;

		m_scripting = &scripting;

		DEBUG_INIT(ScriptCache);
		return status;
	}

	bool ScriptCache::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(ScriptCache);
		return status;
	}

	bool ScriptCache::loadFromFile(const char* file, bool reload)
	{
		assert(file);
		using std::begin; using std::end;
		auto it = std::find_if(begin(m_loadedScripts), end(m_loadedScripts), [&](const std::string& script)
		{
			return script == file;
		});

		bool status = false;
		if(it == end(m_loadedScripts) && !reload)
		{
			m_loadedScripts.emplace_back(file);
			status = m_scripting->doFile(file);
		}
		else if(it == end(m_loadedScripts) && reload)
		{
			DEBUG_INFO("Cannot reload script ", file, " as it wasn't loaded.");
		}
		else if(it != end(m_loadedScripts) && !reload)
		{
			DEBUG_INFO("Cannot load script ", file, ", already exists.");
		}
		else if(it != end(m_loadedScripts) && reload)
		{
			status = m_scripting->doFile(file);
		}
		return status;
	}
}
