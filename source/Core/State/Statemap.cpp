	/*** personal header ***/
#include "Core/State/Statemap.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	State& Statemap::Get(const char* name)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			String message = "Statemap::Get(): Attempted to acquire non-existant State '";
			message += name;
			message += "'.";
			throw std::exception(message.c_str());
		}
		return it->second;
	}

	State& Statemap::Get(const String& name)
	{
		return Get(name.c_str());
	}

	State& Statemap::operator[](const char* name)
	{
		if(Contains(name))
		{
			return Get(name);
		}
		return Insert(name, State());
	}

	State& Statemap::operator[](const String& name)
	{
		return operator[](name.c_str());
	}

	State& Statemap::Insert(const char* name, const State& state)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			it = _cache.emplace(name, state).first;
		}
		else
		{
			it->second = state;
		}
		return it->second;
	}

	State& Statemap::Insert(const String& name, const State& state)
	{
		return Statemap::Insert(name.c_str(), state);
	}

	State& Statemap::Insert(const char* name, State&& state)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			it = _cache.emplace(name, state).first;
		}
		else
		{
			it->second = state;
		}
		return it->second;
	}

	State& Statemap::Insert(const String& name, State&& state)
	{
		return Insert(name.c_str(), state);
	}

	bool Statemap::Contains(const char* name)
	{
		return _cache.find(name) != _cache.end();
	}

	bool Statemap::Contains(const String& name)
	{
		return Contains(name.c_str());
	}

	void Statemap::Remove(const char* name)
	{
		auto it = _cache.find(name);
		if(it != _cache.end())
		{
			_cache.erase(it);
		}
	}

	void Statemap::Remove(const String& name)
	{
		Remove(name.c_str());
	}

	void Statemap::Clear()
	{
		_cache.clear();
	}
}