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
			Insert(pair.first, pair.second->Clone());
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
				Insert(pair.first, pair.second->Clone());
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

	bool Statemap::Contains(const char* name)
	{
		return _states.find(name) != _states.end();
	}

	bool Statemap::Contains(const String& name)
	{
		return _states.find(name) != _states.end();
	}

	void Statemap::Clear()
	{
		_states.clear();
	}

	bool Statemap::Remove(const char* name)
	{
		return _states.erase(name) != 0;
	}

	bool Statemap::Remove(const String& name)
	{
		return _states.erase(name) != 0;
	}
	/*
	bool Statemap::Insert(const char* name, State* state)
	{
		return Insert(name, std::unique_ptr<State>(state));
	}

	bool Statemap::Insert(const String& name, State* state)
	{
		return Insert(name.c_str(), std::unique_ptr<State>(state));
	}
	*/
	bool Statemap::Insert(const char* name, std::unique_ptr<State> state)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			return _states.emplace(std::make_pair(name, std::move(state))).second;
		}
		it->second.swap(state);
		return true;
	}
	
	bool Statemap::Insert(const String& name, std::unique_ptr<State> state)
	{
		return Insert(name.c_str(), std::move(state));
	}
	
	State* Statemap::Retrieve(const char* name)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	State* Statemap::Retrieve(const String& name)
	{
		return Retrieve(name.c_str());
	}

	
}