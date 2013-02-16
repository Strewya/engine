#ifndef CORE_STATE_STATEMAP_H_
#define CORE_STATE_STATEMAP_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "State.h"
	/*** end header inclusion ***/

namespace Core
{
	/*
		Statemap class
		Description:
			Used to map States to a String identifier, as states themselves currently dont have names.
	*/
	class Statemap
	{
	private:
		typedef std::unordered_map<String, State> StateCache;
		StateCache _cache;

	public:
		
		//Gets the State identified by 'name'. Throws an exception if the State doesn't exist.
		State& Get(const char* name);
		//Gets the State identified by 'name'. Throws an exception if the State doesn't exist.
		State& Get(const String& name);
		//Gets the State identified by 'name'. If it doesn't exist, it will create a new one with the specified name.
		State& operator[](const char* name);
		//Gets the State identified by 'name'. If it doesn't exist, it will create a new one with the specified name.
		State& operator[](const String& name);
		//Inserts 'state' in the container under the identifier 'name' using copy semantics. If it already exists, it is replaced.
		State& Insert(const char* name, const State& state);
		//Inserts 'state' in the container under the identifier 'name' using copy semantics. If it already exists, it is replaced.
		State& Insert(const String& name, const State& state);
		//Inserts 'state' in the container under the identifier 'name' using the move semantics. If it already exists, it is replaced.
		State& Insert(const char* name, State&& state);
		//Inserts 'state' in the container under the identifier 'name' using the move semantics. If it already exists, it is replaced.
		State& Insert(const String& name, State&& state);
		//Checks whether a State identified by 'name' exists in the container.
		bool Contains(const char* name) const;
		//Checks whether a State identified by 'name' exists in the container.
		bool Contains(const String& name) const;
		//Removes the state with the identifier 'name' exists in the container. Does nothing if it doesn't exists.
		void Remove(const char* name);
		//Removes the state with the identifier 'name' exists in the container. Does nothing if it doesn't exists.
		void Remove(const String& name);
		//Clears the container of all states.
		void Clear();
		//Inserts 'value' in the container under the identifier 'name' by creating a new State of replacing the value of the existing State object.
		template<typename T> State& Insert(const char* name, const T& value)
		{
			auto it = _cache.find(name);
			if(it == _cache.end())
			{
				it = _cache.emplace(name, value).first;
			}
			else
			{
				it->second = value;
			}
			return it->second;
		}
		//Inserts 'value' in the container under the identifier 'name' by creating a new State of replacing the value of the existing State object.
		template<typename T> State& Insert(const String& name, const T& value)
		{
			return Insert(name.c_str(), value);
		}
		//Inserts 'value' in the container under the identifier 'name' by creating a new State of replacing the value of the existing State object.
		template<typename T> State& Insert(const char* name, T&& value)
		{
			auto it = _cache.find(name);
			if(it == _cache.end())
			{
				it = _cache.emplace(name, value).first;
			}
			else
			{
				it->second = value;
			}
			return it->second;
		}
		//Inserts 'value' in the container under the identifier 'name' by creating a new State of replacing the value of the existing State object.
		template<typename T> State& Insert(const String& name, T&& value)
		{
			return Insert(name.c_str(), value);
		}
	};





	/*
		Statemap class
		Description:
			Contains State classes via their base class BaseState.
			The internal states are allocated dynamically, and stored in a hash map via shared_ptr.
			The map functions like a regular map, with methods to insert, remove and retrieve States.
	*/
	/*
	class Statemap
	{
	private:
		typedef std::unordered_map<String, std::shared_ptr<BaseState>> StateCache;
		StateCache _cache;

	public:
		template<typename T>
		State<T>& Get(const char* name)
		{
			auto it = _cache.find(name);
			if(it == _cache.end())
			{
				throw std::exception();
			}
			return static_cast<State<T>&>(*it->second);
		}

		template<typename T>
		State<T>& Get(const String& name)
		{
			return Get(name.c_str());
		}

		template<typename T>
		State<T>& Insert(const char* name, const State<T>& state)
		{
			std::shared_ptr<State<T>> dataPtr(new State<T>(state));
			auto it = _cache.find(name);
			if(it == _cache.end())
			{
				_cache.insert(std::make_pair(name, dataPtr));
			}
			else
			{
				it->second = dataPtr;
			}
			return *dataPtr;
		}

		template<typename T>
		State<T>& Insert(const String& name, const State<T>& state)
		{
			return Insert(name.c_str(), state);
		}

		void Remove(const String& name)
		{
			auto it = _cache.find(name);
			if(it != _cache.end())
			{
				_cache.erase(it);
			}
		}
	};
	*/
}

#endif //CORE_STATE_STATEMAP_H_