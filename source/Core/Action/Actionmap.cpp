//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Actionmap.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Actionmap::Actionmap()
	{}
		
	bool Actionmap::Contains(const char* name)
	{
		return _cache.find(name) != _cache.end();
	}

	bool Actionmap::Contains(const String& name)
	{
		return Contains(name.c_str());
	}

	void Actionmap::Clear()
	{
		_cache.clear();
	}

	bool Actionmap::Remove(const char* name)
	{
		auto it = _cache.find(name);
		if(it != _cache.end())
		{
			_cache.erase(it);
			return true;
		}
		return false;
	}

	bool Actionmap::Remove(const String& name)
	{
		return Remove(name.c_str());
	}

	bool Actionmap::Insert(const char* name, std::unique_ptr<Action> action)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			return _cache.emplace(std::make_pair(name, std::move(action))).second;
		}
		it->second.swap(action);
		return true;
	}

	bool Actionmap::Insert(const String& name, std::unique_ptr<Action> action)
	{
		return Insert(name.c_str(), std::move(action));
	}

	Action* Actionmap::Retrieve(const char* name)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	Action* Actionmap::Retrieve(const String& name)
	{
		return Retrieve(name.c_str());
	}
}