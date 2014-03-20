#pragma once
/********************************************
*	class:	ConfigFile
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	//we are using the scripting system as the interface for our configurations
	class ScriptingSystem;

	class ConfigFile
	{
	public:
		ConfigFile(ScriptingSystem& state);

		const std::string& getFilename() const;
		std::string getString(const char* key, const char* valueIfNotPresent);

	private:
		ScriptingSystem* m_scripting;
		std::string m_filename;
	};
}
