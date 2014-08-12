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
		WE_INVALID=0,

		WE_KEYBOARDKEY,
		WE_KEYBOARDTEXT,
		
		WE_MOUSEMOVE,
		WE_MOUSEBUTTON,
		WE_MOUSEWHEEL,

		WE_GAMEPADBUTTON,
		WE_GAMEPADAXIS,

		WE_CLOSE,
		WE_LOSTFOCUS,
		WE_GAINFOCUS,

		WE_FILECHANGE,

		WE_LASTEVENT
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
		int32_t m_scroll;
	};

	struct GamepadButtonEvent
	{
		uint8_t m_gamepad;
		uint8_t m_button;
		bool m_isDown;
	};

	struct GamepadAxisEvent
	{
		int16_t m_x;
		int16_t m_y;
		uint8_t m_gamepad;
		uint8_t m_axis;
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
		uint32_t m_index;
	};



	class WindowEvent
	{
	public:
		uint32_t m_type;
		uint64_t m_timestamp;
		union
		{
			KeyboardEvent m_keyboard;
			MouseMoveEvent m_mouseMove;
			MouseButtonEvent m_mouseButton;
			MouseWheelEvent m_mouseWheel;
			FileChangeEvent m_fileChange;
			GamepadButtonEvent m_gamepadButton;
			GamepadAxisEvent m_gamepadAxis;
		};
	};
}
