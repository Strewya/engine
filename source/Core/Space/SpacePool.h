#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
#include <list>
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Space;

	class SpacePool
	{
	public:
		SpacePool(uint32_t maxExpectedSize = 4096);
		
		InstanceID getNewInstance(Space** outSpace);
		InstanceID getNamedInstance(const char* name, Space** outSpace);
		Space* getInstance(InstanceID id) const;
		Space* getInstance(const char* name) const;

		bool isAlive(InstanceID id) const;
		bool isNameInUse(const char* name) const;

		bool destroy(InstanceID id);
		bool destroy(const char* name);

		bool attachName(const char* name, InstanceID id);
		bool detachName(const char* name, InstanceID id);
		
	private:
		typedef std::deque<std::unique_ptr<Space>> PoolData;
		PoolData _pool;
		std::list<uint32_t> _availableSlots;
		std::unordered_map<String, InstanceID> _names;
		uint32_t _maxExpectedSize;
		uint32_t _indexBits;
		uint32_t _indexMask;
		InstanceID _idCounter;
		InstanceID _newID(int index);
	};
}