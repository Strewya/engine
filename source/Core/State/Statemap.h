#ifndef CORE_STATE_STATEMAP_H_
#define CORE_STATE_STATEMAP_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"

#include <memory>
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

		typedef std::unordered_map<String, std::unique_ptr<IState>> StateCache;
		StateCache _states;

		Statemap* _prototype;

	public:
		Statemap();
		Statemap(const Statemap& rhs);
		Statemap(Statemap&& rhs);
		Statemap& operator=(const Statemap& rhs);
		Statemap& operator=(Statemap&& rhs);

		void setPrototype(Statemap& prototype);

		bool hasState(const char* name, bool recursive = false);
		bool hasState(const String& name, bool recursive = false);

		void ClearStates();
		bool RemoveState(const char* name);
		bool RemoveState(const String& name);

		bool Insert(const char* name, IState* state);
		bool Insert(const String& name, IState* state);
		bool Insert(const char* name, std::unique_ptr<IState> state);
		bool Insert(const String& name, std::unique_ptr<IState> state);

		IState* getState(const char* name);
		IState* getState(const String& name);

		

		template<typename T> T& getValue(const char* name)
		{
			auto it = _states.find(name);
			if(it == _states.end())
			{
				if(_prototype != nullptr)
				{
					return _prototype->getValue<T>(name);
				}
				else
				{
					throw std::exception(String("Missing state ").append(name).append(" in Statemap::GetValue()").c_str());
				}
			}
			return it->second->as<T>();
		}

		template<typename T> T& getValue(const String& name)
		{
			return getValue<T>(name.c_str());
		}
	};
}

#endif //CORE_STATE_STATEMAP_H_