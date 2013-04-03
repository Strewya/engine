//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Input/InputEngine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Win32/AbstractWindow.h"
	/*** end headers ***/

namespace Input
{
	Engine::Engine(Win32::AbstractWindow& window)
		: _keys(Keyboard::_KeyCount, false), _mouseKeys(Mouse::_KeyCount, false)
	{
		window.setEventQueue(_eventQueue);
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
				Util::GetDefaultLogger() << "Input::Engine::Update() - Undefined event type encoutered in eventQueue: " << (int)event.type << Util::Logger::endl;
				continue;
			}
		}
	}

	void Engine::PurgeOldEvents()
	{
		_eventQueue.clear();
	}

	bool Engine::isKeyPressed(Keyboard::Keys key) const
	{
		return _keys[key];
	}

	bool Engine::isMouseButtonPressed(Mouse::Keys button) const
	{
		return _mouseKeys[button];
	}

	bool Engine::PollEvent(Event& out)
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