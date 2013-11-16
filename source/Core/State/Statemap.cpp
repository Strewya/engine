	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Core/State/Statemap.h>
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
			insert(pair.second->clone());
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
				insert(pair.second->clone());
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

	bool Statemap::contains(InstanceID id)
	{
		return _states.find(id) != _states.end();
	}

	void Statemap::clear()
	{
		_states.clear();
	}

	bool Statemap::destroy(InstanceID id)
	{
		return _states.erase(id) != 0;
	}
	
	bool Statemap::insert(StateUptr state)
	{
		auto it = _states.find(state->uid);
		if(it == _states.end())
		{
			return _states.emplace(std::make_pair(state->uid, std::move(state))).second;
		}
		it->second.swap(state);
		return true;
	}
	
	StateRptr Statemap::retrieve(InstanceID id)
	{
		auto it = _states.find(id);
		if(it == _states.end())
		{
			return nullptr;
		}
		return it->second.get();
	}
}