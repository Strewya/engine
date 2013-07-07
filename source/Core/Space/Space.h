#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
#include <functional>
#include <unordered_set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class EntityPool;

	class Space
	{
	public:
		typedef std::unordered_set<InstanceID> EntityContainer;
		typedef EntityContainer::iterator iterator;
	

	public:
		Space(InstanceID id);
		Space(InstanceID id, const Util::Vec2& pos, const Util::Vec2& size);
		Space(InstanceID id, const Util::Rect& bounds);
		~Space();

		InstanceID getID() const;
		void setBounds(const Util::Rect& bounds);
		void setBounds(const Util::Vec2& pos, const Util::Vec2& size);
		const Util::Rect& getBounds() const;

		bool AddEntity(InstanceID id);
		bool RemoveEntity(InstanceID id);
		bool FindEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const;
		iterator begin() const;
		iterator end() const;
	
	private:
		InstanceID _id;
		Util::Rect _bounds;
		EntityContainer _entities;
	};
}