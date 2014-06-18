#pragma once
/********************************************
*	class:	WindowEvent
*	usage:
********************************************/
/******* common header *******/
#include <Engine/Defines.h>
/******* extra headers if needed *******/
/******* end header inclusion *******/

namespace Core
{
	enum WindowEventType
	{
		WINDOW_INVALID=0,

		WINDOW_KEYDOWN,
		WINDOW_KEYUP,
		WINDOW_TEXT,

		WINDOW_MOUSEMOVE,
		WINDOW_MOUSEBUTTONDOWN,
		WINDOW_MOUSEBUTTONUP,
		WINDOW_MOUSEWHEEL,

		WINDOW_LASTEVENT
	};

	struct CommonEvent
	{
		uint32_t m_type;
		uint32_t m_timestamp;
	};

	struct KeyboardEvent
	{
		uint32_t m_type;
		uint32_t m_timestamp;
		uint32_t m_keyCode;
		bool m_isDown;
		uint8_t m_repeat;
	};

	struct MouseMoveEvent
	{
		uint32_t m_type;
		uint32_t m_timestamp;
		int32_t m_x;
		int32_t m_y;
		int32_t m_xrel;
		int32_t m_yrel;
	};




	union WindowEvent
	{
		uint32_t m_type;
		CommonEvent m_common;

	};
}
