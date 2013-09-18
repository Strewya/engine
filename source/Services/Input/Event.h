#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Input
{
	enum class EventType
	{
		KeyPressed,
		KeyReleased,
		TextEntered,
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
		uint32_t code;
		bool alt;
		bool control;
		bool shift;
	};

	struct TextEvent
	{
		char16_t character;
	};

	struct MouseMoveEvent
	{
		bool relative;
		uint32_t x;
		uint32_t y;
	};

	struct MouseButtonEvent
	{
		uint32_t button;
	};

	struct MouseWheelEvent
	{
		int delta;
	};

	struct ResizeEvent
	{
		uint32_t width;
		uint32_t height;
	};

	class Event
	{
	public:
		EventType type;
		//timestamp needed here
		union
		{
			KeyEvent key;
			TextEvent text;
			MouseMoveEvent mouseMove;
			MouseButtonEvent mouseButton;
			MouseWheelEvent mouseWheel;
			ResizeEvent resize;
		};
	};

	bool operator==(const Event& lhs, const Event& rhs);
}