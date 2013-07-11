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
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Entity/Entity.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	/*
		The InstanceID of entities in the pool is as follows:
		the left bits are unique to the entity
		the right bits are the index in the list 
	*/
	class EntityPool
	{
	public:
		EntityPool(uint32_t maxExpectedSize = 4096);
		
		InstanceID getNewInstance(Entity** outEntity = nullptr);
		Entity* getInstance(InstanceID id) const;
		bool isAlive(InstanceID id) const;
		bool destroy(InstanceID id);
		
	private:
		std::deque<std::unique_ptr<Entity>> _pool;
		std::list<uint32_t> _availableSlots;
		uint32_t _maxExpectedSize;
		uint32_t _indexBits;
		uint32_t _indexMask;
		InstanceID _idCounter;
		InstanceID _newID(int index);
	};
}