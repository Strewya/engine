	/*** personal header ***/
#include "Core/State/Statemap.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Statemap::Statemap()
	{
	}

	Statemap::Statemap(const Statemap& rhs)
	{
		for(auto& pair : rhs._states)
		{
			insert(pair.first, pair.second->clone());
		}
	}

	Statemap::Statemap(Statemap&& rhs)
		: _states(std::move(rhs._states))
	{
	}

	Statemap& Statemap::operator=(const Statemap& rhs)
	{
		if(this != &rhs)
		{
			for(auto& pair : rhs._states)
			{
				insert(pair.first, pair.second->clone());
			}
		}
		return *this;
	}

	Statemap& Statemap::operator=(Statemap&& rhs)
	{
		if(this != &rhs)
		{
			_states = std::move(rhs._states);
			
			rhs._states.clear();
		}
		return *this;
	}

	bool Statemap::contains(const char* name)
	{
		return _states.find(name) != _states.end();
	}

	bool Statemap::contains(const String& name)
	{
		return _states.find(name) != _states.end();
	}

	void Statemap::clear()
	{
		_states.clear();
	}

	bool Statemap::destroy(const char* name)
	{
		return _states.erase(name) != 0;
	}

	bool Statemap::destroy(const String& name)
	{
		return _states.erase(name) != 0;
	}
	
	bool Statemap::insert(const char* name, StateUptr state)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			return _states.emplace(std::make_pair(name, std::move(state))).second;
		}
		it->second.swap(state);
		return true;
	}
	
	bool Statemap::insert(const String& name, StateUptr state)
	{
		return insert(name.c_str(), std::move(state));
	}
	
	StateRptr Statemap::retrieve(const char* name)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	StateRptr Statemap::retrieve(const String& name)
	{
		return retrieve(name.c_str());
	}

	
}