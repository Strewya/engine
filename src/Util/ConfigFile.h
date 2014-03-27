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
#include <Util/Color.h>
/******* end header inclusion *******/

namespace Core
{
	//we are using the scripting system as the interface for our configurations
	class ScriptingSystem;

	class ConfigFile
	{
	public:
		ConfigFile(ScriptingSystem& state);

		bool open(const char* filename);
		void close();
		const std::string& getFilename() const;
		
		std::string getString(const char* key, const char* valueIfNotPresent);
		std::string getString(const char* valueIfNotPresent);
		int32_t getInt(const char* key, int32_t valueIfNotPresent);
		int32_t getInt(int32_t defaultValue);
		float getFloat(const char* key, float valueIfNotPresent);
		float getFloat(float defaultValue);
		Vec2 getVec2(const char* key, Vec2 valueIfNotPresent);
		Vec2 getVec2(Vec2 defaultValue);
		bool getBool(const char* key, bool valueIfNotPresent);
		bool getBool(bool defaultValue);
		
		uint32_t getListSize(const char* list);
		bool getListElement(const char* list, uint32_t element);
		void popListElement();

	private:
		ScriptingSystem* m_scripting;
		bool m_isOpen;
		std::string m_filename;
		
	};
}
