#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <deque>
#include <functional>
#include <memory>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/Entity/Entity.h>
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
		
		InstanceID getNewInstanceID();
		Entity& getNewInstanceRef();
		Entity* getInstancePtr(InstanceID id) const;
		Entity& getInstanceRef(InstanceID id) const;
		bool isAlive(InstanceID id) const;
		bool destroy(InstanceID id);

		void registerDestructionCallback(std::function<void(InstanceID)> callback);
		
	private:
		InstanceID newID(int index);
		void dispatchDestruction(Entity& e);

		std::deque<std::unique_ptr<Entity>> m_pool;
		std::deque<uint32_t> m_availableSlots;
		uint32_t m_maxExpectedSize;
		uint32_t m_indexBits;
		uint32_t m_indexMask;
		InstanceID m_idCounter;
		std::function<void(InstanceID)> m_onDestroyCallback;
		

		
	};
}