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

	enum class ActionFamily
	{
		Atomic,
		Composite
	};
	/**************************************************************************
	*		BASE ACTION CLASS
	**************************************************************************/
	
	class Action
	{
	public:
		virtual ~Action() {};
		virtual InstanceID getUID() const = 0;
		virtual ActionFamily getFamily() const = 0;
		virtual void init(GameContext& context) = 0;
		virtual void checkMessages(GameContext& context) = 0;
		virtual void processMessage(GameContext& context, uint32_t msg, InstanceID entity) = 0;
		virtual void update(GameContext& context) = 0;
		virtual void render(GameContext& context, uint64_t interpolationTime) = 0;
		virtual void addAction(ActionRptr action) = 0;
		
	};


	/**************************************************************************
	*		TEMPLATED ATOMIC ACTION CLASS
	*	
	*	Derived classes must implement these methods:
	*		void processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	*		void update(GameContext& context)
	*		void render(GameContext& context, uint64_t interpolationTime)
	*	
	*	Derived classes should implement these methods if custom behaviour is desired:
	*		void init(GameContext& context)
	*		void onAddEntity(GameContext& context, InstanceID entity)
	*		void onRemoveEntity(GameContext& context, InstanceID entity)
	*		void onDestroyEntity(GameContext& context, InstanceID entity)
	*		bool validateEntity(GameContext& context, InstanceID eid)
	*
	**************************************************************************/
	template<typename T> class AtomicAction : public Action
	{
	public:
		typedef T Derived_t;
		typedef std::unique_ptr<Derived_t> Uptr;
		typedef Derived_t* Rptr;
		typedef Derived_t& Ref;
		
	public:
		static const InstanceID UID;
		static const ActionFamily Family;
		static Uptr create(GameContext& c);

	public:
		InstanceID getUID() const;
		ActionFamily getFamily() const;
		virtual void init(GameContext& context);
		void checkMessages(GameContext& context);
		void addAction(ActionRptr action);
		
		virtual void onAddEntity(GameContext& context, InstanceID entity) {};
		virtual void onRemoveEntity(GameContext& context, InstanceID entity) {};
		virtual void onDestroyEntity(GameContext& context, InstanceID entity) {};
		virtual bool validateEntity(GameContext& context, InstanceID eid) { return true; }
		
		bool defaultMessageHandler(GameContext& context, uint32_t msgId, InstanceID target);
	protected:
		typedef std::unordered_set<InstanceID> EntityStorage_t;
		EntityStorage_t m_entities;
		uint32_t m_entityAddMsgCode;
		uint32_t m_entityRemoveMsgCode;
		uint32_t m_entityDestroyMsgCode;
	};


	/**************************************************************************
	*		COMPOSITE ACTION CLASS
	*	
	*	Derived classes must implement these methods:
	*		void update(GameContext& context)
	*		void render(GameContext& context, uint64_t interpolationTime)
	*	
	*	Derived classes should implement these methods if custom behaviour is desired:
	*		void init(GameContext& context)
	*
	**************************************************************************/
	template<typename T> class CompositeAction : public Action
	{
	public:
		typedef T Derived_t;
		typedef std::unique_ptr<Derived_t> Uptr;
		typedef Derived_t* Rptr;
		typedef Derived_t& Ref;
		
	public:
		static const InstanceID UID;
		static const ActionFamily Family;
		static Uptr create(GameContext& c);

	public:
		InstanceID getUID() const;
		ActionFamily getFamily() const;
		virtual void init(GameContext& context);
		void checkMessages(GameContext& context);
		void processMessage(GameContext& context, uint32_t msg, InstanceID entity);
		void addAction(ActionRptr action);

	protected:
		typedef std::vector<ActionRptr> ActionStorage_t;
		ActionStorage_t m_actions;
	};


	/* FOR CONVENIENCE */
	class Multiaction : public CompositeAction<Multiaction>
	{
	public:
		void update(GameContext& context);
		void render(GameContext& context, uint64_t interpolationTime);
	};

	/**************************************************************************
	*		ATOMIC ACTION IMPLEMENTATION
	**************************************************************************/

	template<typename T> const InstanceID AtomicAction<T>::UID = typeid(T).hash_code();
	template<typename T> const InstanceID CompositeAction<T>::UID = typeid(T).hash_code();

	template<typename T> const ActionFamily AtomicAction<T>::Family = ActionFamily::Atomic;
	template<typename T> const ActionFamily CompositeAction<T>::Family = ActionFamily::Composite;
	
	template<typename T> auto AtomicAction<T>::create(GameContext& c) -> Uptr
	{
		return Uptr(new Derived_t());
	}

	template<typename T> InstanceID AtomicAction<T>::getUID() const
	{
		return UID;
	}

	template<typename T> ActionFamily AtomicAction<T>::getFamily() const
	{
		return Family;
	}

	template<typename T> void AtomicAction<T>::init(GameContext& context)
	{
		m_entityAddMsgCode = context.m_messenger.registerMessage("entity_add");
		m_entityRemoveMsgCode = context.m_messenger.registerMessage("entity_remove");
		m_entityDestroyMsgCode = context.m_messenger.registerMessage("entity_destroy");
	}

	template<typename T> void AtomicAction<T>::checkMessages(GameContext& context)
	{
		uint32_t msg;
		InstanceID entity;
		while(context.m_messenger.consumeMessage(getUID(), msg, entity) && !defaultMessageHandler(context, msg, entity))
		{
			processMessage(context, msg, entity);
		}
	}

	template<typename T> void AtomicAction<T>::addAction(ActionRptr action)
	{}

	template<typename T> bool AtomicAction<T>::defaultMessageHandler(GameContext& context, uint32_t msgId, InstanceID target)
	{
		bool handled = true;
		if(msgId == m_entityAddMsgCode)
		{
			if(validateEntity(context, target))
			{
				m_entities.emplace(target);
				onAddEntity(context, target);
			}
		}
		else if(msgId == m_entityRemoveMsgCode)
		{
			if(m_entities.erase(target) == 1)
			{
				onRemoveEntity(context, target);
			}
		}
		else if(msgId == m_entityDestroyMsgCode)
		{
			if(m_entities.erase(target) == 1)
			{
				onDestroyEntity(context, target);
			}
		}
		else
			handled = false;
		return handled;
	}


	/**************************************************************************
	*		COMPOSITE ACTION IMPLEMENTATION
	**************************************************************************/
	template<typename T> auto CompositeAction<T>::create(GameContext& c) -> Uptr
	{
		Uptr ptr(new Derived_t());
		ptr->init(c);
		return ptr;
	}

	template<typename T> InstanceID CompositeAction<T>::getUID() const
	{
		return UID;
	}

	template<typename T> ActionFamily CompositeAction<T>::getFamily() const
	{
		return Family;
	}

	template<typename T> void CompositeAction<T>::init(GameContext& context)
	{
		for(auto& action : m_actions)
		{
			action->init(context);
		}
	}

	template<typename T> void CompositeAction<T>::checkMessages(GameContext& context)
	{
		for(auto& action : m_actions)
		{
			action->checkMessages(context);
		}
	}

	template<typename T> void CompositeAction<T>::processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	{
		for(auto& action : m_actions)
		{
			action->processMessage(context, msg, entity);
		}
	}

	template<typename T> void CompositeAction<T>::addAction(ActionRptr action)
	{
		m_actions.emplace_back(action);
	}
}