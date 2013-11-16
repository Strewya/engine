#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <deque>
#include <functional>
#include <unordered_set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Util/Dimensional.h>
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class EntityPool;

	class Space
	{
	public:
		typedef std::unordered_set<InstanceID> EntityContainer_t;
		typedef EntityContainer_t::iterator iterator;
		typedef EntityContainer_t::const_iterator const_iterator;
	

	public:
		Space(InstanceID id);
		~Space();

		InstanceID getID() const;
		
		bool addEntity(InstanceID id);
		bool removeEntity(InstanceID id);
		bool findEntities(std::deque<InstanceID>& out, const EntityPool& pool, std::function<bool(const Entity&)> filter) const;
		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		void clear();

	private:
		InstanceID m_id;
		EntityContainer_t m_entities;
	};
}
