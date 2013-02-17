#ifndef CORE_ACTION_ACTION_H_
#define CORE_ACTION_ACTION_H_
/********************************************
	class:	Action
	usage:	Each Entity class has instances of this class contained within.
			Each instance of Action has a function pointer as it's own member.
			The use pattern for the Action is:
				* Action is created and the function pointer is set for it
				* Action is added to an Entity's list of possible actions
				* Action is started, and it's member 'active' is set to true
					- this is a mere simplification, and is used instead of
					  moving the action from an 'inactive' to an 'active' list
				* when started, the Action's owning Entity is added to a list 
				  of all Entities that are being processed for that function pointer
				* an Action queue is set so that it invokes the function pointers in a certain order
				* when an action is invoked, it's passed the list of entities to process, 
				  the resource locator and the subsystem locator objects
			Updating the action:
				* the Engine calls context.Update(dt);
				* the Context invokes Update on the ActionUpdater object
				* the ActionUpdater object has a priority sorted list of pairs <function pointer, entities to process>
				* for each pair the function pointer is invoked by passing in the list of entities, the subsystems locator and the resources locator
				* sexyyyyyyyyyyy
				* 
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

typedef void(*Func)(/*const ResourceLocator& resources, const ServiceLocator& services,*/ const std::set<Core::Entity*>& ents);

namespace Core
{
	class Action
	{
	private:
		Func _actionLogic;
		bool _active;
		Entity* _owner;
		Entity* _target;

	public:
		Action();
		Action(Func fn, Entity& owner);

		void operator()(float deltaTime);
		void Update(float deltaTime);

		void setFunction(Func fn);
		
		bool isActive() const;

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