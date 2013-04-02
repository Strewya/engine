#ifndef SUBSYSTEMS_INPUT_INPUTSYSTEM_H_
#define SUBSYSTEMS_INPUT_INPUTSYSTEM_H_
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include "Defines.h"
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
		std::list<Event> _eventQueue;
		
	public:
		Engine();
		
		void Update(Win32::AbstractWindow& window);
		void PurgeOldEvents();

		bool isKeyPressed(uint keyCode) const;
	};
}

#endif //SUBSYSTEMS_INPUT_INPUTSYSTEM_H_