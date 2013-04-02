//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Input/InputEngine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Win32/AbstractWindow.h"
	/*** end headers ***/

namespace Input
{
	Engine::Engine()
	{
		_keys.resize(256, false);
	}

	void Engine::Update(Win32::AbstractWindow& window)
	{
		for(auto& event : _eventQueue)
		{
			switch(event.type)
			{
			case EventType::KeyPressed:
				_keys[event.key.code] = true;
				_keys[Key::_Alt] = event.key.alt;
				_keys[Key::_Control] = event.key.control;
				_keys[Key::_Shift] = event.key.shift;
			break;

			case EventType::KeyReleased:
				_keys[event.key.code] = false;
				_keys[Key::_Alt] = event.key.alt;
				_keys[Key::_Control] = event.key.control;
				_keys[Key::_Shift] = event.key.shift;
			break;

			default:
				continue;
			}
		}
	}

	void Engine::PurgeOldEvents()
	{
		_eventQueue.clear();
	}

	bool Engine::isKeyPressed(uint keyCode) const
	{
		return _keys[keyCode] != 0;
	}
}