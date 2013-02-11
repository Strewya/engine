#ifndef CORE_ACTION_ACTION_H_
#define CORE_ACTION_ACTION_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
}

typedef void(*Func)(const std::set<Core::Entity>&);

namespace Core
{
	class Action
	{
	public:
		Action();
		Action(Func fn);

		void operator()(float deltaTime);
		void Update(float deltaTime);


	private:
		Func _actionLogic;

		std::set<Entity> _entities;
	};




	
	/*
	//fwds
	class ActionPriorityQueue;
	//end fwds

	//
	//	INTERFACE CLASS
	//		ActionContainer
	//
	class ActionContainer
	{
	public:
		typedef std::list<Action*> List;
		virtual bool Enqueue(Action* a, bool listen) = 0;
		virtual bool Dequeue(Action* a) = 0;
	};


	//
	//	BASE CLASS
	//		Action
	//
	class Action : public ServiceHolder
	{
	public:
		typedef ActionContainer::List::iterator Iterator;
	private:
		static InstanceID _idCounter;
		static InstanceID _GenerateID();
		static shared_ptr<ActionPriorityQueue> _defaultQueue;
		
		InstanceID _id;
		const String _type;
		uint _priority;
		String _alias;
		bool _active;
		Entity& _parent;
		Action* _listener;
		Iterator _qPos;
		ulong _current;
		ulong _total;
		ActionContainer* _container;

		Entity* _caller;
		Entity* _target;
		spSpace _targetSpace;

		Action(const Action& rhs);
		Action& operator=(const Action& rhs);
		
	protected:
		Action(Entity& owner, const String& type);
		Action(Entity& owner, ticpp::Element& xml);

		virtual void OnUpdate(ulong time) = 0;
		virtual void OnInput(MappedInput& input);
		virtual void OnCallback(Action* action);

	public:
		static Action& getDefaultQueue();
		static void setDefaultQueue(spAction c);

		virtual ~Action();

		Action& Update(ulong time);
		Action& Input(MappedInput& input);
		Action& Callback(Action* action);
		Action& Start();
		Action& Done();

		bool Tick();
		bool Time(ulong dt);

		Action& setCaller(Entity& e);
		Action& setTarget(Entity& e);
		Action& setTarget(spSpace s);
		Action& setActive(const bool& status);
		Action& setTotal(const ulong& total);
		Action& setListener(Action& listener);
		Action& setIterator(const Iterator& it);
		Action& setAlias(const String& alias);
		Action& setPriority(const uint& priority);
		Action& setQueue(ActionContainer& cont);
		
		Action& ClearListener();

		Entity&			getParent() const;
		bool			isActive() const;
		Action&			getListener() const;
		Iterator		getIterator() const;
		const String&	getType() const;
		String			getAlias() const;
		uint			getPriority() const;
		InstanceID		getID() const;
	};

	//
	//	TEMPLATE CLASS
	//		ActionTemplate
	//
	template<class T>
	class ActionTemplate : public Action
	{
	private:
		ActionTemplate(const ActionTemplate& rhs) : Action(rhs) {}
		ActionTemplate& operator=(const ActionTemplate& rhs) { Action::operator=(rhs); return *this; }


		template<class T>
		class ActionTemplateMaker : public FactoryRouter<Action>
		{
			static ActionTemplateMaker _self;
		public:
			ActionTemplateMaker()
				: FactoryRouter(ActionTemplate<T>::_type)
			{}

			spAction MakeType(ticpp::Element& xml, Entity& owner)
			{
				return T::Make(owner, xml);
			}
		};

		static const String _type;

	protected:
		typedef ActionTemplateMaker<T> Maker;

		ActionTemplate(Entity& owner) : Action(owner, _type) {}
		ActionTemplate(Entity& owner, ticpp::Element& xml) : Action(owner, xml) {}

	public:
		~ActionTemplate() {}

		static T* Convert(Action* ptr)
		{
			if(ptr == NULL || _type != ptr->getType())
			{
				Logger() << "Bad Action cast: " << (ptr == NULL ? "NULL" : ptr->getType()) << " attempted to cast to " << _type << "." << Logger::endline;
				return NULL;
			}
			return static_cast<T*>(ptr);
		}
	};

	*/
}

#endif //CORE_ACTION_ACTION_H_