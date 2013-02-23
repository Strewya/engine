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

	}

	Statemap::Statemap(Statemap&& rhs)
		: _prototype(rhs._prototype), _cache(std::move(rhs._cache))
	{
	}

	Statemap& Statemap::operator=(const Statemap& rhs)
	{
		if(this != &rhs)
		{
			_prototype = rhs._prototype;
		}
		return *this;
	}

	Statemap& Statemap::operator=(Statemap&& rhs)
	{
		if(this != &rhs)
		{
			_prototype = rhs._prototype;
			_cache = std::move(rhs._cache);
		}
		return *this;
	}

	void Statemap::SetPrototype(Statemap& prototype)
	{
		_prototype = &prototype;
	}

	bool Statemap::Contains(const char* name)
	{
		return _cache.find(name) != _cache.end();
	}

	bool Statemap::Contains(const String& name)
	{
		return Contains(name.c_str());
	}

	bool Statemap::RemoveState(const char* name)
	{
		auto it = _cache.find(name);
		if(it != _cache.end())
		{
			_cache.erase(it);
			return true;
		}
		return false;
	}

	bool Statemap::RemoveState(const String& name)
	{
		return RemoveState(name.c_str());
	}
	
	bool Statemap::AddState(const char* name, IState* state)
	{
		return AddState(name, std::unique_ptr<IState>(state));
	}

	bool Statemap::AddState(const String& name, IState* state)
	{
		return AddState(name.c_str(), std::unique_ptr<IState>(state));
	}
	
	bool Statemap::AddState(const char* name, std::unique_ptr<IState> state)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			return _cache.emplace(std::make_pair(name, std::move(state))).second;
		}
		it->second.swap(state);
		return true;
	}
	
	bool Statemap::AddState(const String& name, std::unique_ptr<IState> state)
	{
		return AddState(name.c_str(), std::move(state));
	}
	
	IState* Statemap::GetState(const char* name)
	{
		auto it = _cache.find(name);
		if(it == _cache.end())
		{
			if(_prototype)
			{
				return _prototype->GetState(name);
			}
			return nullptr;
		}
		return it->second.get();
	}

	IState* Statemap::GetState(const String& name)
	{
		return GetState(name.c_str());
	}

	void Statemap::Clear()
	{
		_cache.clear();
	}
}