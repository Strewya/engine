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
		typedef EntityContainer::iterator Iterator;
	private:
		EntityContainer _entities;
		Util::Rect _bounds;
		InstanceID _id;

	public:
		Space();
		Space(const Util::Vec2& pos, const Util::Vec2& size);
		Space(const Util::Rect& bounds);
		~Space();

		InstanceID getID() const;
		void setBounds(const Util::Rect& bounds);
		void setBounds(const Util::Vec2& pos, const Util::Vec2& size);
		const Util::Rect& getBounds() const;

		bool AddEntity(InstanceID id);
		bool RemoveEntity(InstanceID id);
		uint32_t FindEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const;
		Iterator begin() const;
		bool isEnd(const Iterator& it) const;
		
	};
}