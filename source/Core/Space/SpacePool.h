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
	/*** end header inclusion ***/

namespace Core
{
	class Space;

	class SpacePool
	{
	private:
		std::deque<std::unique_ptr<Space>> _pool;
		std::list<uint32_t> _availableSlots;
		uint32_t _maxExpectedSize;
		uint32_t _indexBits;
		uint32_t _indexMask;
		InstanceID _idCounter;
		InstanceID _NewID(int index);

	public:
		SpacePool(uint32_t maxExpectedSize = 4096);
		
		Space& NewInstance();
		Space& Retrieve(InstanceID id) const;
		bool IsAlive(InstanceID id) const;
		bool Destroy(InstanceID id);
	};
}