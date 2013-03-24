#ifndef CORE_ENTITY_ENTITYPOOL_H_
#define CORE_ENTITY_ENTITYPOOL_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <deque>
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Entity;

	class EntityPool
	{
	private:
		std::deque<std::shared_ptr<Entity>> _entityList;

	public:
		Entity* getEntityByAlias(const String& alias) const;
		Entity* getNewEntity();
	};
}

#endif //CORE_ENTITY_ENTITYPOOL_H_