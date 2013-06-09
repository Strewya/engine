#pragma once
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Subsystems/Input/KeyCodes.h"
	/*** extra headers if needed ***/
#include "Subsystems/Input/Event.h"
	/*** end header inclusion ***/

namespace Win32 { class AbstractWindow; }

namespace Input
{
	class Engine
	{
	private:
		std::vector<bool> _keys;
		std::vector<bool> _mouseKeys;

		std::list<Event> _eventQueue;
		
	public:
		Engine(Win32::AbstractWindow& window);
		
		void Update();
		void PurgeEvents();

		bool isPressed(Keyboard::Keys key) const;
		bool isPressed(Mouse::Keys button) const;
		bool PollEvent(Event& out);
	};
}