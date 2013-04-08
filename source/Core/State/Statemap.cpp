	/*** personal header ***/
#include "Core/State/Statemap.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Statemap::Statemap()
		: _prototype(nullptr)
	{
	}

	Statemap::Statemap(const Statemap& rhs)
		: _prototype(rhs._prototype)
	{
		for(auto& pair : rhs._states)
		{
			Insert(pair.first, pair.second->Clone());
		}
	}

	Statemap::Statemap(Statemap&& rhs)
		: _prototype(rhs._prototype), _states(std::move(rhs._states))
	{
	}

	Statemap& Statemap::operator=(const Statemap& rhs)
	{
		if(this != &rhs)
		{
			_prototype = rhs._prototype;
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
			_prototype = rhs._prototype;
			_states = std::move(rhs._states);
			
			rhs._prototype = nullptr;
			rhs._states.clear();
		}
		return *this;
	}

	void Statemap::setPrototype(Statemap& prototype)
	{
		_prototype = &prototype;
	}

	bool Statemap::hasState(const char* name, bool recursive)
	{
		bool found = _states.find(name) != _states.end();
		if(!found && recursive && _prototype != nullptr)
		{
			return _prototype->hasState(name, recursive);
		}
		return found;
	}

	bool Statemap::hasState(const String& name, bool recursive)
	{
		return hasState(name.c_str());
	}

	void Statemap::ClearStates()
	{
		_states.clear();
	}

	bool Statemap::RemoveState(const char* name)
	{
		return _states.erase(name) != 0;
	}

	bool Statemap::RemoveState(const String& name)
	{
		return _states.erase(name) != 0;
	}
	
	bool Statemap::Insert(const char* name, State* state)
	{
		return Insert(name, std::unique_ptr<State>(state));
	}

	bool Statemap::Insert(const String& name, State* state)
	{
		return Insert(name.c_str(), std::unique_ptr<State>(state));
	}
	
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
	
	State* Statemap::getState(const char* name)
	{
		auto it = _states.find(name);
		if(it == _states.end())
		{
			if(_prototype)
			{
				return _prototype->getState(name);
			}
			return nullptr;
		}
		return it->second.get();
	}

	State* Statemap::getState(const String& name)
	{
		return getState(name.c_str());
	}

	
}