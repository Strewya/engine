//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Actionmap.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Actionmap::Actionmap(Entity& owner)
		: _owner(owner)
	{
	}

	Action& Actionmap::Insert(const char* name, const Action& action)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			it = _cache.emplace(name, action).first;
		}
		else
		{
			it->second = action;
		}
		it->second.setOwner(_owner);
		return it->second;
	}

	Action& Actionmap::Insert(const String& name, const Action& action)
	{
		return Insert(name.c_str(), action);
	}

	Action& Actionmap::Get(const char* name)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			String message = "Actionmap::Get(): Attempted to acquire non-existant Action '";
			message += name;
			message += "'.";
			throw std::exception(message.c_str());
		}
		return it->second;
	}

	Action& Actionmap::Get(const String& name)
	{
		return Get(name.c_str());
	}
}