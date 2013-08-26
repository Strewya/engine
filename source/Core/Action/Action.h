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
		Action(InstanceID type, GameContext& context);

		void update();

		bool registerEntity(InstanceID id);
		bool unregisterEntity(InstanceID id);
		
		const InstanceID uid;
	protected:

		virtual void onUpdate(float dt) = 0;

		typedef std::unordered_set<InstanceID> EntityStorage_t;
		EntityStorage_t m_entities;
		Util::Timer m_timer;
		GameContext& m_context;
	};


	template<typename T> struct ActionType : public Action
	{
		typedef T Derived_t;
		typedef std::unique_ptr<Derived_t> Uptr;
		typedef Derived_t* Rptr;
		typedef Derived_t& Ref;

		static const InstanceID Type;

		static std::unique_ptr<Derived_t> create(GameContext& c)
		{
			return std::unique_ptr<Derived_t>(new Derived_t(c));
		}

	protected:
		ActionType(GameContext& context) : Action(Type, context) {}
		virtual void init() {}
	};

	template<typename T> const InstanceID ActionType<T>::Type = typeid(T).hash_code();
}

#define SYSTEM(Name) class Name : public ActionType<Name>
#define SYSTEM_CTOR(Name) Name(GameContext& c) : ActionType(c) { init(); }