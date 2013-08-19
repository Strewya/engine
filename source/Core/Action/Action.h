#pragma once
/********************************************
	class:	Action
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <unordered_set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Timer.h"
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;

	class Action
	{
	public:
		Action(InstanceID type);

		bool update(float dt, GameContext& context);

		bool registerEntity(InstanceID id);
		bool unregisterEntity(InstanceID id);
		
		const InstanceID uid;
	protected:
		virtual bool onUpdate(float dt, GameContext& context) = 0;

		typedef std::unordered_set<InstanceID> EntityStorage_t;
		EntityStorage_t _entities;
		Util::Timer _timer;
	};


	template<typename T> struct ActionType : public Action
	{
		typedef T Derived_t;
		typedef std::unique_ptr<Derived_t> Uptr;
		typedef Derived_t* Rptr;
		typedef Derived_t& Ref;

		static const InstanceID Type;

		static std::unique_ptr<Derived_t> create()
		{
			return std::unique_ptr<Derived_t>(new Derived_t());
		}

	protected:
		ActionType() : Action(Type) {}
	};

	template<typename T> const InstanceID ActionType<T>::Type = typeid(T).hash_code();
}

#define SYSTEM(Name) class Name : public ActionType<Name>