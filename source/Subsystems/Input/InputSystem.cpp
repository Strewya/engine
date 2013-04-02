	/*** personal header ***/
#include "SGInputSystem.h"
	/*** extra headers ***/
#include "SGGameAction.h"
	/*** end headers ***/

namespace SG
{
	InputSystem::InputSystem()
	{	
		//must initialize contexts
		_LoadContexts("keybindings.xml");
		
		ContextMap::iterator it = _contexts.find("default");
		if(it != _contexts.end())
			_activeContext = _contexts["default"].get();
		else
			throw std::exception("There is no defualt context available!");
	}

	InputSystem::~InputSystem()
	{
	}
	
	void InputSystem::_LoadContexts(const String& file)
	{
		try
		{
			ticpp::Document doc(file);
			doc.LoadFile();

			ticpp::Iterator< ticpp::Element > it;
			for( it = it.begin(doc.FirstChildElement()); it != it.end(); ++it)
			{
				String name = it->GetAttribute("name");
				_contexts[name].reset( new InputContext(it->ToElement()) );
			}

		}
		catch(ticpp::Exception& ex)
		{
			String error = "Error parsing file " + file + " in InputSystem constructor, terminating.";
			Logger() << error << Logger::endline;
			Logger() << ex.what() << Logger::endline;

			throw std::exception(error.c_str());
		}
	}
	
	void InputSystem::Dispatch()
	{
		if(_currentInput.IsEmpty())
			return;
		
		CallbackTable::const_iterator it = _callbacks.begin();
		for(; it != _callbacks.end(); ++it)
		{
			it->second->Input(_currentInput);
		}
	}

	bool InputSystem::PushContext(const String& name)
	{
		ContextMap::iterator it = _contexts.find(name);
		if(it == _contexts.end())
			return false;

		_activeContext = _activeContext->Push(it->second.get());
		return true;
	}

	bool InputSystem::PopContext()
	{
		_activeContext = _activeContext->Pop();
		
		return true;
	}

	void InputSystem::RegisterCallback(Action* callback, int priority)
	{
		_callbacks.insert( std::make_pair( priority, callback ) );
	}

	void InputSystem::Clear()
	{
		_currentInput.ClearActions();
		_currentInput.ClearRanges();
	}

	void InputSystem::SetButtonState(uint button, bool pressed, bool repeated)
	{
		String name;
		
		if(pressed && !repeated)
		{
			if(_MapButtonToAction(button, name))
			{
				_currentInput.AddAction(name);
				return;
			}
		}

		if(pressed)
		{
			if(_MapButtonToState(button, name))
			{
				_currentInput.AddState(name);
				return;
			}
		}

		_ClearButton(button);
	}
	
	void InputSystem::SetAxisValue(uint axis, double value)
	{
		String name;
		if(_activeContext->MapAxisToRange(axis, name))
		{
			_currentInput.AddRange(name, value); //this should use the converter to normalize the value to an interval
		}
	}

	bool InputSystem::_MapButtonToState(uint button, String& outState)
	{
		if(_activeContext->MapButtonToState(button, outState))
			return true;
		return false;
	}

	bool InputSystem::_MapButtonToAction(uint button, String& outAction)
	{
		if(_activeContext->MapButtonToAction(button, outAction))
			return true;
		return false;
	}

	void InputSystem::_ClearButton(uint button)
	{
		String name;
		
		if(_MapButtonToAction(button, name))
			_currentInput.EatAction(name);

		if(_MapButtonToState(button, name))
			_currentInput.EatState(name);
	}
}