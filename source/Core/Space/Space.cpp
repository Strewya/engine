	/*** personal header ***/
#include "SGGameSpace.h"
	/*** extra headers ***/
#include "SGGameEntity.h"
#include "SGGameForm.h"
	/*** end headers ***/

namespace SG
{
	InstanceID Space::_idCounter = 0;
	InstanceID Space::_GenerateID()
	{
		return _idCounter++;
	}


	Cell::Cell(const Vector& pos, const Vector& size)
		: _bounds(Rect(pos, size)), _entities()
	{
		_curIterator = _entities.end();
	}

	Cell::~Cell()
	{
		clear();
	}

	void Cell::Insert(spEntity e)
	{
		_entities.insert(e);
	}

	void Cell::Insert(Cell& c)
	{
		_entities.insert(c._entities.begin(), c._entities.end());
	}

	void Cell::Remove(spEntity e)
	{
		_entities.erase(e);
	}

	spEntity Cell::begin()
	{
		_curIterator = _entities.begin();
		return *_curIterator;
	}

	spEntity Cell::next()
	{
		++_curIterator;
		return *_curIterator;
	}
	
	bool Cell::end() const
	{
		return _curIterator == _entities.end();
	}

	bool Cell::empty() const
	{
		return _entities.empty();
	}

	uint Cell::count() const
	{
		return _entities.size();
	}

	void Cell::clear()
	{
		_entities.clear();
		/*while(!_entities.empty())
		{
			Entity* e = _entities.begin()->get();
			if(_parent != NULL && e->getLocation().getID() == _parent->getID())
			{
				e->UnsetLocation().UnsetParent();
			}
			_entities.erase(_entities.begin());
		}*/
	}




	Space::Space(Entity& owner, double w, double h, int x, int y)
		: _id(_GenerateID()), _parent(owner), _width(w), _height(h), _cellsX(x == 0 ? 1 : x), _cellsY(y == 0 ? 1 : y), _cellSizeX(w/(x == 0 ? 1 : x)), _cellSizeY(h/(y == 0 ? 1 : y))
	{
		_area.reserve(x*y);
		Vector cell((float)_cellSizeX, (float)_cellSizeY);
		for(int j=0; j<y; ++j)
		{
			for(int i=0; i<x; ++i)
			{
				_area.push_back(Cell(Vector((float)(i*_cellSizeX), (float)(j*_cellSizeY)), cell));
			}
		}
	}

	Space::~Space()
	{
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			spEntity e = cell->begin();
			while(!cell->end() && e->getLocation() != NULL && e->getLocation()->getID() == _id)
			{
				e->UnsetLocation().UnsetParent();
			}
		}

		clear();
	}

	InstanceID Space::getID() const
	{
		return _id;
	}

	Entity& Space::getParentEntity() const
	{
		return _parent;
	}

	Form& Space::getParentForm() const
	{
		return _parent.getForm();
	}

	double Space::getWidth() const
	{
		return _width;
	}

	double Space::getHeight() const
	{
		return _height;
	}

	int Space::getCellsX() const
	{
		return _cellsX;
	}

	int Space::getCellsY() const
	{
		return _cellsY;
	}

	Vector Space::getCellsSize() const
	{
		return Vector((float)_cellSizeX, (float)_cellSizeY);
	}

	bool Space::empty()
	{
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			if(!cell->empty())
				return false;
		}
		return true;
	}

	void Space::clear()
	{
		Area::iterator it = _area.begin();
		for(; it != _area.end(); ++it)
		{
			it->clear();
		}
	}
	bool Space::Insert(spEntity e, bool child)
	{
		if(e == NULL)
			return false;

		if(e->getLocation() == this)
			return false;

		Vector pos = e->getForm().getPosition();

		
		_area[_PosToIndex(pos)].Insert(e);

		if(child)
			e->setLocation(this).setParent(&_parent);
		
		return true;
	}

	bool Space::Update(spEntity e, const Vector& oldPos)
	{
		if(e == NULL)
			return false;

		int old_idx = _PosToIndex(oldPos);
		int new_idx = _PosToIndex(e->getForm().getPosition());
		if(old_idx == new_idx)
			return false;

		_area[old_idx].Remove(e);
		_area[new_idx].Insert(e);
		return true;
	}

	bool Space::Remove(spEntity e)
	{
		if(e == NULL)
			return false;
		
		Vector pos = e->getForm().getPosition();
		_area[_PosToIndex(pos)].Remove(e);
		if(e->getLocation() == this)
		{
			e->UnsetLocation();
			e->UnsetParent();
		}
		
		return true;
	}

	int	Space::_PosToIndex(const Vector& pos)
	{
		uint idx = (int)(pos.x/_cellSizeX) + (int)(pos.y/_cellSizeY)*_cellsX;
		if(idx >= _area.size())
			idx = _area.size()-1;
		return (int)idx;
	}

	Cell& Space::GetCell(int x, int y)
	{
		uint idx = y*_cellsX + x;
		if(idx < 0)
			idx = 0;
		if(idx >= _area.size())
			idx = _area.size()-1;
		return _area[idx];
	}

	shared_ptr<Cell> Space::GetAll()
	{
		shared_ptr<Cell> c(new Cell(Vector(), Vector(_width, _height)));
		Area::iterator it = _area.begin();
		for(; it != _area.end(); ++it)
		{
			c->Insert(*it);
		}
		return c;
	}

	spEntity Space::FindFirstByType(const String& type, bool recursive)
	{
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			spEntity e = cell->begin();
			while(!cell->end())
			{
				if(e->getType() == type)
				{
					return e;
				}
				if(recursive)
				{
					spEntity r = e->getForm().getContents().FindFirstByType(type, recursive);
					if(r != NULL)
						return r;
				}
				e = cell->next();
			}
		}
		return NULL;
	}

	Cell Space::FindAllByType(const String& type, bool recursive)
	{
		Cell c(Vector(), Vector(_width, _height));
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			spEntity e = cell->begin();
			while(!cell->end())
			{
				if(e->getType() == type)
				{
					c.Insert(e);
				}
				if(recursive)
				{
					Cell r = e->getForm().getContents().FindAllByType(type, recursive);
					c.Insert(r);
				}
				e = cell->next();
			}
		}
		return c;
	}

	spEntity Space::FindFirstByAlias(const String& alias, bool recursive)
	{
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			spEntity e = cell->begin();
			while(!cell->end())
			{
				if(e->getAlias() == alias)
				{
					return e;
				}
				if(recursive)
				{
					spEntity r = e->getForm().getContents().FindFirstByAlias(alias, recursive);
					if(r != NULL)
						return r;
				}
				e = cell->next();
			}
		}
		return NULL;
	}

	Cell Space::FindAllByAlias(const String& alias, bool recursive)
	{
		Cell c(Vector(), Vector(_width, _height));
		Area::iterator cell = _area.begin();
		for(; cell != _area.end(); ++cell)
		{
			spEntity e = cell->begin();
			while(!cell->end())
			{
				if(e->getAlias() == alias)
				{
					c.Insert(e);
				}
				if(recursive)
				{
					Cell r = e->getForm().getContents().FindAllByAlias(alias, recursive);
					c.Insert(r);
				}
				e = cell->next();
			}
		}
		return c;
	}
}