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
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;

	class Action
	{
	public:
		const InstanceID uid;

		Action(InstanceID type, GameContext& context);
		virtual ~Action();

		void update();

		bool registerEntity(InstanceID id);
		bool unregisterEntity(InstanceID id);
		bool validateEntity(InstanceID id);
		virtual void registerCallbacks();
		
	protected:
		typedef std::unordered_set<InstanceID> EntityStorage_t;

		GameContext& m_context;
		bool m_timerExpired;
		int32_t m_timerId;
		EntityStorage_t m_entities;
		
		virtual bool validateEntity(Entity& entity) = 0;
		virtual void onUpdate(float dt) = 0;
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

#define SYSTEM(Name) class Name : public Core::ActionType<Name>
#define SYSTEM_CTOR(Name) Name(Core::GameContext& c) : ActionType(c) { init(); }