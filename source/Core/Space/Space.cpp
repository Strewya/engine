//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Space/Space.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/EntityPool.h"
	/*** end headers ***/

namespace Core
{
	Space::Space()
		: _bounds()
	{
	}

	Space::Space(const Util::Rect& bounds)
		: _bounds(bounds)
	{
	}

	Space::Space(const Util::Vec2& pos, const Util::Vec2& size)
		: _bounds(pos, size)
	{
	}

	Space::~Space()
	{
	}

	InstanceID Space::getID() const
	{
		return _id;
	}

	const Util::Rect& Space::getBounds() const
	{
		return _bounds;
	}

	void Space::setBounds(const Util::Rect& bounds)
	{
		_bounds = bounds;
	}

	void Space::setBounds(const Util::Vec2& pos, const Util::Vec2& size)
	{
		_bounds = Util::Rect(pos, size);
	}

	bool Space::AddEntity(InstanceID id)
	{
		return _entities.insert(id).second;
	}

	bool Space::RemoveEntity(InstanceID id)
	{
		return _entities.erase(id) != 0;
	}

	uint Space::FindEntities(std::deque<InstanceID>& container, const EntityPool& pool, std::function<bool(const Entity&)> filter) const
	{
		uint count = 0;
		for(auto& id : _entities)
		{
			if(filter(pool.Retrieve(id)))
			{
				container.push_back(id);
				++count;
			}
		}
		return count;
	}
	
	Space::Iterator Space::begin() const
	{
		return _entities.begin();
	}
	
	bool Space::isEnd(const Iterator& it) const
	{
		return it == _entities.end();
	}
}