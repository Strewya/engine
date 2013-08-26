//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/InputEngine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Input
{
	Engine::Engine()
		: _keys(Keyboard::_KeyCount, false), _mouseKeys(Mouse::_KeyCount, false)
	{
	}

	void Engine::Update()
	{
		for(auto& event : _eventQueue)
		{
			switch(event.type)
			{
			case EventType::KeyPressed:
				_keys[event.key.code] = true;
				_keys[Keyboard::_Alt] = event.key.alt;
				_keys[Keyboard::_Control] = event.key.control;
				_keys[Keyboard::_Shift] = event.key.shift;
			break;

			case EventType::KeyReleased:
				_keys[event.key.code] = false;
				_keys[Keyboard::_Alt] = event.key.alt;
				_keys[Keyboard::_Control] = event.key.control;
				_keys[Keyboard::_Shift] = event.key.shift;
			break;

			case EventType::MouseButtonPressed:
				_mouseKeys[event.mouseButton.button] = true;
			break;

			case EventType::MouseButtonReleased:
				_mouseKeys[event.mouseButton.button] = false;
			break;

			default:
			break;
			}
		}
	}

	void Engine::PurgeEvents()
	{
		_eventQueue.clear();
	}

	std::list<Event>& Engine::getEventQueue()
	{
		return _eventQueue;
	}

	bool Engine::isPressed(Keyboard::Keys key) const
	{
		if(key>=0 && key<Keyboard::_KeyCount)
			return _keys[key];
		return false;
	}

	bool Engine::isPressed(Mouse::Keys button) const
	{
		if(button>=0 && button<Mouse::_KeyCount)
			return _mouseKeys[button];
		return false;
	}

	bool Engine::EatEvent(Event& out)
	{
		if(!_eventQueue.empty())
		{
			out = _eventQueue.front();
			_eventQueue.pop_front();
			return true;
		}
		return false;
	}
}
