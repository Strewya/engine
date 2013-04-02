#ifndef SUBSYSTEMS_INPUT_EVENT_H_
#define SUBSYSTEMS_INPUT_EVENT_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Input
{
	enum class EventType
	{
		KeyPressed,
		KeyReleased,
		MouseMoved,
		MouseWheelMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		LostFocus,
		GainedFocus,
		Resized
	};

	struct KeyEvent
	{
		uint code;
		bool alt;
		bool control;
		bool shift;
	};

	struct MouseMoveEvent
	{
		uint dx;
		uint dy;
	};

	struct MouseButtonEvent
	{
		uint button;
	};

	struct MouseWheelEvent
	{
		int delta;
	};

	struct ResizeEvent
	{
		uint width;
		uint height;
	};

	class Event
	{
	public:
		EventType type;

		union
		{
			KeyEvent key;
			MouseMoveEvent mouseMove;
			MouseButtonEvent mouseButton;
			MouseWheelEvent mouseWheel;
			ResizeEvent resize;
		};
	};
}

#endif //SUBSYSTEMS_INPUT_EVENT_H_