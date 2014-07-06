#pragma once
/********************************************
*	class:	ScriptCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;
	class ScriptingSystem;

	class ScriptCache
	{
	public:
		bool init(ScriptingSystem& scripting);
		bool shutdown();

		bool loadFromFile(const ResourceFile& file, bool reload);

	private:
		ScriptingSystem* m_scripting;
		std::vector<std::string> m_loadedScripts;
	};
}
