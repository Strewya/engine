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
	class ResourceFile;
	class ScriptingSystem;

	class DataFile
	{
	public:
		DataFile(ScriptingSystem& state);

		bool open(const ResourceFile& filename);
		void close();
		const std::string& getFilename() const;
		

		std::string getString(const char* key, const char* valueIfNotPresent);
		std::string getString(uint32_t index, const char* valueIfNotPresent);
		char getChar(const char* key, char valueIfNotPresent);
		char getChar(uint32_t index, char valueIfNotPresent);
		int32_t getInt(const char* key, int32_t valueIfNotPresent);
		int32_t getInt(uint32_t index, int32_t defaultValue);
		float getFloat(const char* key, float valueIfNotPresent);
		float getFloat(uint32_t index, float defaultValue);
		Vec2 getVec2(const char* key, Vec2 valueIfNotPresent);
		Vec2 getVec2(uint32_t index, Vec2 defaultValue);
		bool getBool(const char* key, bool valueIfNotPresent);
		bool getBool(uint32_t index, bool defaultValue);
		
		uint32_t getTableSize(const char* list);
		bool getTable(const char* list);
		bool getTable(uint32_t index);
		void popTable();

		void pairs(const char* table);
		void ipairs(const char* table);
		bool next();
		
	private:
		ScriptingSystem* m_scripting;
		bool m_isOpen;
		std::string m_filename;
		enum class IterateType
		{
			NONE,
			PAIRS,
			IPAIRS
		};
		IterateType m_iteration;
	};
}
