#pragma once
/********************************************
*	class:	WindowEvent
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
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

		WINDOW_CLOSE,
		WINDOW_LOSTFOCUS,
		WINDOW_GAINFOCUS,

		WINDOW_FILECHANGE,

		WINDOW_LASTEVENT
	};

	struct KeyboardEvent
	{
		uint8_t m_keyCode;
		uint8_t m_repeat;
		bool m_isDown;
		bool m_previouslyDown;

	};

	struct MouseMoveEvent
	{
		int32_t m_x;
		int32_t m_y;
		bool m_isRelative;
	};

	struct MouseButtonEvent
	{
		int32_t m_x;
		int32_t m_y;
		uint8_t m_button;
		uint8_t m_clicks;
		bool m_isDown;
	};

	struct MouseWheelEvent
	{
		int32_t m_x;
		int32_t m_y;
		uint32_t m_scroll;
	};

	enum FileChangeType
	{
		FILE_ADDED,
		FILE_MODIFIED,
		FILE_REMOVED,
		FILE_RENAMED_FROM,
		FILE_RENAMED_TO,
		FILE_BADDATA
	};

	struct FileChangeEvent
	{
		uint32_t m_action;
	};



	struct WindowEvent
	{
		uint32_t m_type;
		uint64_t m_timestamp;
		union
		{
			KeyboardEvent m_keyboard;
			MouseMoveEvent m_mouseMove;
			MouseButtonEvent m_mouseButton;
			MouseWheelEvent m_mouseWheel;
			FileChangeEvent m_fileChange;
		};
	};
}
