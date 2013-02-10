	/* personal header */
#include "SGActionmap.h"
	/* extra headers */
#include "SGAction.h"
	/* end headers */

namespace SG
{
	Actionmap::Actionmap()
		: _container()
	{
	}

	Actionmap::~Actionmap()
	{
		Clear();
	}

	bool Actionmap::Insert(spAction a, bool replace)
	{
		if(a == NULL)
		{
			Logger() << "NULL action passed to Actionmap." << Logger::endline;
			return false;
		}

		Container::iterator it = _container.find(a->getAlias());
		if(it == _container.end())
		{
			return _container.insert( std::make_pair(a->getAlias(), a) ).second;
		}

		if(replace)
		{
			it->second = a;
			return true;
		}
		return false;
	}

	Action* Actionmap::Get(const String& type)
	{
		return Acquire(type).get();
	}
	
	Action* Actionmap::operator[](const String& type)
	{
		return Get(type);
	}

	spAction Actionmap::Acquire(const String& type)
	{
		Container::iterator found = _container.find(type);
		if(found != _container.end())
			return found->second;
		return spAction();
	}
	
	bool Actionmap::Exists(const String& type) const
	{
		return _container.find(type) != _container.end();
	}

	bool Actionmap::Remove(const String& type)
	{
		Container::iterator it = _container.find(type);
		if(it == _container.end())
			return false;
		_container.erase(it);
		return true;
	}

	void Actionmap::Clear()
	{
		_container.clear();
	}
}