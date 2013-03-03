#ifndef CORE_STATE_STATEMAP_H_
#define CORE_STATE_STATEMAP_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"

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
		StateCache _cache;

		Statemap* _prototype;

	public:
		Statemap();
		Statemap(const Statemap& rhs);
		Statemap(Statemap&& rhs);
		Statemap& operator=(const Statemap& rhs);
		Statemap& operator=(Statemap&& rhs);

		void SetPrototype(Statemap& prototype);

		bool Contains(const char* name);
		bool Contains(const String& name);

		bool RemoveState(const char* name);
		bool RemoveState(const String& name);

		bool AddState(const char* name, IState* state);
		bool AddState(const String& name, IState* state);
		bool AddState(const char* name, std::unique_ptr<IState> state);
		bool AddState(const String& name, std::unique_ptr<IState> state);

		IState* GetState(const char* name);
		IState* GetState(const String& name);

		void Clear();

		template<typename T> T& GetValue(const char* name)
		{
			auto it = _cache.find(name);
			if(it == _cache.end())
			{
				if(_prototype != nullptr)
				{
					_prototype->GetValue<T>(name);
				}
				else
				{
					throw std::exception(String("Missing state ").append(name).append(" in Statemap::GetValue()").c_str());
				}
			}
			return it->second->as<T>();
		}

		template<typename T> T& GetValue(const String& name)
		{
			return GetValue<T>(name.c_str());
		}
	};
}

#endif //CORE_STATE_STATEMAP_H_