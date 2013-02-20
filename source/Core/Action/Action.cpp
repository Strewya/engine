//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/


namespace Core
{
	Action::Action()
		: _actionLogic(nullptr)
	{}

	Action::Action(Func fn)
		: _actionLogic(fn)
	{}

	void Action::Update(float dt)
	{
		if(_actionLogic)
		{
			//_actionLogic(_entities);
		}
	}

	void Action::operator()(float dt)
	{
		Update(dt);
	}






	/*
	InstanceID Action::_idCounter = 0;
	shared_ptr<ActionPriorityQueue> Action::_defaultQueue;

	InstanceID Action::_GenerateID()
	{
		return ++_idCounter;
	}

	Action::Action(Entity& owner, const String& type)
		: _id(_GenerateID()), _type(type), _priority(Priority_First), _alias(type), _active(false), _parent(owner), _listener(NULL),
		_current(0), _total(1), _container(_defaultQueue.get()), _qPos(), _caller(NULL), _target(NULL), _targetSpace(NULL)
	{
	}

	Action::Action(Entity& owner, ticpp::Element& xml)
		: _id(_GenerateID()), _type(xml.GetAttribute("type")), _priority(Priority_First), _alias(_type), _active(false), _parent(owner), _listener(NULL),
		_qPos(), _current(0), _total(1), _container(_defaultQueue.get()), _caller(NULL), _target(NULL), _targetSpace(NULL)
	{
		try
		{
			// _id
			// _type
			// _priority
			xml.GetAttribute("priority", &_priority, false);
			// _alias
			xml.GetAttribute("name", &_alias, false);
			// _active == autostart
			xml.GetAttribute("autostart", &_active, false);
			if(_active)
			{ Start(); }
			// _parent
			// _listener
			// _qPos
			// _current
			// _total
			xml.GetAttribute("total", &_total, false);
			// _container
			// _caller
			// _target
			// _targetSpace
		}
		catch(ticpp::Exception& ex)
		{
			Log("Error while parsing general Action information.\n");
			Log(ex.what());
			throw ex;
		}
	}

	Action::~Action()
	{
		if(_active) Done();
	}

	Action& Action::Update(ulong dt)
	{
		if(_active)
			OnUpdate(dt);
		return *this;
	}

	Action& Action::Callback(Action* action)
	{
		if(action != NULL)
			OnCallback(action);
		return *this;
	}

	Action& Action::Start()
	{
		if(_active == false)
		{
			_active = _container->Enqueue(this, true);
		}
		return *this;
	}

	Action& Action::Done()
	{
		_active = false;
		// not sure if this should be here, or not
		//_container->Dequeue(this);
		if(_listener != NULL)
			_listener->Callback(this);
		return *this;
	}

	Action& Action::Input(MappedInput& input)
	{
		OnInput(input);
		return *this;
	}

	bool Action::Tick()
	{
		++_current;
		if(_current < _total)
			return false;
		_current = 0;
		return true;
	}

	bool Action::Time(ulong dt)
	{
		_current += dt;
		if(_current < _total)
			return false;
		_current -= _total;
		return true;
	}

	// ------------------------------------- SETTERS ------------------------//
	void Action::setDefaultQueue(spAction c)
	{
		if(_defaultQueue == NULL)
		{
			_defaultQueue = c;
		}
	}

	Action& Action::setCaller(Entity& e)
	{
		_caller = &e;
		return *this;
	}

	Action& Action::setTarget(Entity& e)
	{
		_target = &e;
		return *this;
	}

	Action& Action::setTarget(spSpace s)
	{
		_targetSpace = s;
		return *this;
	}
	
	Action& Action::setActive(const bool& status)
	{
		_active = status;
		return *this;
	}
	
	Action& Action::setTotal(const ulong& total)
	{
		_total = total;
		return *this;
	}

	Action& Action::setListener(Action& listener)
	{
		_listener = &listener;
		return *this;
	}

	Action& Action::ClearListener()
	{
		_listener = NULL;
		return *this;
	}

	Action& Action::setIterator(const std::list<Action*>::iterator& it)
	{
		_qPos = it;
		return *this;
	}

	Action& Action::setAlias(const String& alias)
	{
		_alias = alias;
		return *this;
	}

	Action& Action::setPriority(const uint& priority)
	{
		_priority = priority;
		return *this;
	}

	Action& Action::setQueue(ActionContainer& cont)
	{
		_container = &cont;
		return *this;
	}
	// ------------------------------------------ SETTERS END ----------------------------------//


	// ------------------------------------------ GETTERS -----------------------//
	Action& Action::getDefaultQueue()
	{
		return *_defaultQueue;
	}
	
	Entity& Action::getParent() const
	{
		return _parent;
	}
	
	bool Action::isActive() const
	{
		return _active;
	}

	Action& Action::getListener() const
	{
		return *_listener;
	}
	
	std::list<Action*>::iterator Action::getIterator() const
	{
		return _qPos;
	}

	const String& Action::getType() const
	{
		return _type;
	}
	
	String Action::getAlias() const
	{
		return _alias;
	}

	uint Action::getPriority() const
	{
		return _priority;
	}
	
	InstanceID Action::getID() const
	{
		return _id;
	}
	// --------------------------------------------------- GETTERS END ---------------------------//


	void Action::OnInput(MappedInput& input)
	{}
	
	void Action::OnCallback(Action* action)
	{}

	*/
}