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
		std::list<uint> _availableSlots;
		uint _maxExpectedSize;
		uint _indexBits;
		uint _indexMask;
		InstanceID _idCounter;
		InstanceID _NewID(int index);

	public:
		SpacePool(uint maxExpectedSize = 4096);
		
		Space& NewInstance();
		Space& Retrieve(InstanceID id) const;
		bool IsAlive(InstanceID id) const;
		bool Destroy(InstanceID id);
	};
}