#ifndef CORE_ENTITY_ENTITYPOOL_H_
#define CORE_ENTITY_ENTITYPOOL_H_
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
	private:
		std::deque<std::unique_ptr<Entity>> _entities;
		std::list<uint> _availableSlots;
		uint _maxExpectedEntities;
		uint _indexBits;
		uint _indexMask;
		InstanceID _idCounter;
		InstanceID _NewID(int index);

	public:
		EntityPool(uint maxExpectedEntities = 4096);
		
		Entity& NewEntity();
		Entity& GetEntity(InstanceID id) const;
		bool IsAlive(InstanceID id) const;
		bool DestroyEntity(InstanceID id);
	};
}

#endif //CORE_ENTITY_ENTITYPOOL_H_