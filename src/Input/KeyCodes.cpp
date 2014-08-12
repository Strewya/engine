//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Input/KeyCodes.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Keyboard::Map Keyboard::m_map;
	Mouse::Map Mouse::m_map;
	Gamepad::Map Gamepad::m_map;

	void InitializeInputConstants()
	{
		Keyboard::Init();
		Mouse::Init();
		Gamepad::Init();
	}

	int32_t Gamepad::Code(const std::string& name)
	{
		Map::iterator it = m_map.begin();
		Map::iterator end = m_map.end();
		for(; it != end; ++it)
		{
			if(it->second == name)
				return it->first;
		}
		return m_Unknown;
	}

	std::string Gamepad::Name(uint32_t code)
	{
		Map::iterator it = m_map.find(code);
		return (it != m_map.end() ? it->second : "");
	}

	void Gamepad::Init()
	{
		m_map.emplace(Gamepad::m_DPadUp, "DPadUp");
		m_map.emplace(Gamepad::m_DPadDown, "DPadDown");
		m_map.emplace(Gamepad::m_DPadLeft, "DPadLeft");
		m_map.emplace(Gamepad::m_DPadRight, "DPadRight");
		m_map.emplace(Gamepad::m_LeftButton, "LeftButton");
		m_map.emplace(Gamepad::m_TopButton, "TopButton");
		m_map.emplace(Gamepad::m_RightButton, "RightButton");
		m_map.emplace(Gamepad::m_BottomButton, "BottomButton");
		m_map.emplace(Gamepad::m_LeftThumb, "LeftThumb");
		m_map.emplace(Gamepad::m_RightThumb, "RightThumb");
		m_map.emplace(Gamepad::m_LeftShoulder, "LeftShoulder");
		m_map.emplace(Gamepad::m_RightShoulder, "RightShoulder");
		m_map.emplace(Gamepad::m_Start, "Start");
		m_map.emplace(Gamepad::m_Back, "Back");
		m_map.emplace(Gamepad::m_LeftStick, "LeftStick");
		m_map.emplace(Gamepad::m_RightStick, "RightStick");
		m_map.emplace(Gamepad::m_LeftTrigger, "LeftTrigger");
		m_map.emplace(Gamepad::m_RightTrigger, "RightTrigger");
	}

	int32_t Mouse::Code(const std::string& name)
	{
		Map::iterator it = m_map.begin();
		Map::iterator end = m_map.end();
		for(; it != end; ++it)
		{
			if(it->second == name)
				return it->first;
		}
		return m_Unknown;
	}

	std::string Mouse::Name(uint32_t code)
	{
		Map::iterator it = m_map.find(code);
		return (it != m_map.end() ? it->second : "" );
	}
	
	void Mouse::Init()
	{
		m_map.insert( std::make_pair( Mouse::m_LeftButton, "LeftButton" ) );
		m_map.insert( std::make_pair( Mouse::m_RightButton, "RightButton" ) );
		m_map.insert( std::make_pair( Mouse::m_MiddleButton, "MiddleButton" ) );
		m_map.insert( std::make_pair( Mouse::m_XButton1, "XButton1" ) );
		m_map.insert( std::make_pair( Mouse::m_XButton2, "XButton2" ) );
	}
	
	int32_t Keyboard::Code(const std::string& name)
	{
		Map::iterator it = m_map.begin();
		Map::iterator end = m_map.end();
		for(; it != end; ++it)
		{
			if(it->second == name)
				return it->first;
		}
		return m_Unknown;
	}

	std::string Keyboard::Name(uint32_t code)
	{
		Map::iterator it = m_map.find(code);
		return (it != m_map.end() ? it->second : "" );
	}
	
	void Keyboard::Init()
	{
		m_map.insert( std::make_pair( Keyboard::m_Cancel, "Cancel" ) );
		
		/*
		* 0x07 : unassigned
		*/

		m_map.insert( std::make_pair( Keyboard::m_Backspace, "Backspace" ) );
		m_map.insert( std::make_pair( Keyboard::m_Tab, "Tab" ) );

		/*
		* 0x0A - 0x0B : reserved
		*/

		m_map.insert( std::make_pair( Keyboard::m_Clear, "Clear" ) );
		m_map.insert( std::make_pair( Keyboard::m_Enter, "Enter" ) );

		m_map.insert( std::make_pair( Keyboard::m_Shift, "Shift" ) );
		m_map.insert( std::make_pair( Keyboard::m_Control, "Control" ) );
		m_map.insert( std::make_pair( Keyboard::m_Alt, "Alt" ) );
		m_map.insert( std::make_pair( Keyboard::m_Pause, "Pause" ) );
		m_map.insert( std::make_pair( Keyboard::m_CapsLock, "CapsLock" ) );
		/*
		m_map.insert( std::make_pair( Keyboard::m_KANA, "KANA" ) );
		m_map.insert( std::make_pair( Keyboard::m_HANGEUL, "HANGEUL" ) );
		m_map.insert( std::make_pair( Keyboard::m_HANGUL, "HANGUL" ) );
		m_map.insert( std::make_pair( Keyboard::m_JUNJA, "JUNJA" ) );
		m_map.insert( std::make_pair( Keyboard::m_FINAL, "FINAL" ) );
		m_map.insert( std::make_pair( Keyboard::m_HANJA, "HANJA" ) );
		m_map.insert( std::make_pair( Keyboard::m_KANJI, "KANJI" ) );
		*/
		m_map.insert( std::make_pair( Keyboard::m_Escape, "Escape" ) );
		/*
		m_map.insert( std::make_pair( Keyboard::m_CONVERT, "CONVERT" ) );
		m_map.insert( std::make_pair( Keyboard::m_NONCONVERT, "NONCONVERT" ) );
		m_map.insert( std::make_pair( Keyboard::m_ACCEPT, "ACCEPT" ) );
		m_map.insert( std::make_pair( Keyboard::m_MODECHANGE, "MODECHANGE" ) );
		*/
		m_map.insert( std::make_pair( Keyboard::m_Space, "Space" ) );
		m_map.insert( std::make_pair( Keyboard::m_PageUp, "PageUp" ) );
		m_map.insert( std::make_pair( Keyboard::m_PageDown, "PageDown" ) );
		m_map.insert( std::make_pair( Keyboard::m_End, "End" ) );
		m_map.insert( std::make_pair( Keyboard::m_Home, "Home" ) );
		m_map.insert( std::make_pair( Keyboard::m_ArrowLeft, "ArrowLeft" ) );
		m_map.insert( std::make_pair( Keyboard::m_ArrowUp, "ArrowUp" ) );
		m_map.insert( std::make_pair( Keyboard::m_ArrowRight, "ArrowRight" ) );
		m_map.insert( std::make_pair( Keyboard::m_ArrowDown, "ArrowDown" ) );
//		m_map.insert( std::make_pair( Keyboard::m_Select, "Select" ) );
//		m_map.insert( std::make_pair( Keyboard::m_Print, "Print" ) );
//		m_map.insert( std::make_pair( Keyboard::m_Execute, "Execute" ) );
		m_map.insert( std::make_pair( Keyboard::m_PrintScreen, "PrintScreen" ) );
		m_map.insert( std::make_pair( Keyboard::m_Insert, "Insert" ) );
		m_map.insert( std::make_pair( Keyboard::m_Delete, "Delete" ) );
//		m_map.insert( std::make_pair( Keyboard::m_HELP, "HELP" ) );

		m_map.insert( std::make_pair( Keyboard::m_0, "0" ) );
		m_map.insert( std::make_pair( Keyboard::m_1, "1" ) );
		m_map.insert( std::make_pair( Keyboard::m_2, "2" ) );
		m_map.insert( std::make_pair( Keyboard::m_3, "3" ) );
		m_map.insert( std::make_pair( Keyboard::m_4, "4" ) );
		m_map.insert( std::make_pair( Keyboard::m_5, "5" ) );
		m_map.insert( std::make_pair( Keyboard::m_6, "6" ) );
		m_map.insert( std::make_pair( Keyboard::m_7, "7" ) );
		m_map.insert( std::make_pair( Keyboard::m_8, "8" ) );
		m_map.insert( std::make_pair( Keyboard::m_9, "9" ) );

		m_map.insert( std::make_pair( Keyboard::m_A, "A" ) );
		m_map.insert( std::make_pair( Keyboard::m_B, "B" ) );
		m_map.insert( std::make_pair( Keyboard::m_C, "C" ) );
		m_map.insert( std::make_pair( Keyboard::m_D, "D" ) );
		m_map.insert( std::make_pair( Keyboard::m_E, "E" ) );
		m_map.insert( std::make_pair( Keyboard::m_F, "F" ) );
		m_map.insert( std::make_pair( Keyboard::m_G, "G" ) );
		m_map.insert( std::make_pair( Keyboard::m_H, "H" ) );
		m_map.insert( std::make_pair( Keyboard::m_I, "I" ) );
		m_map.insert( std::make_pair( Keyboard::m_J, "J" ) );
		m_map.insert( std::make_pair( Keyboard::m_K, "K" ) );
		m_map.insert( std::make_pair( Keyboard::m_L, "L" ) );
		m_map.insert( std::make_pair( Keyboard::m_M, "M" ) );
		m_map.insert( std::make_pair( Keyboard::m_N, "N" ) );
		m_map.insert( std::make_pair( Keyboard::m_O, "O" ) );
		m_map.insert( std::make_pair( Keyboard::m_P, "P" ) );
		m_map.insert( std::make_pair( Keyboard::m_Q, "Q" ) );
		m_map.insert( std::make_pair( Keyboard::m_R, "R" ) );
		m_map.insert( std::make_pair( Keyboard::m_S, "S" ) );
		m_map.insert( std::make_pair( Keyboard::m_T, "T" ) );
		m_map.insert( std::make_pair( Keyboard::m_U, "U" ) );
		m_map.insert( std::make_pair( Keyboard::m_V, "V" ) );
		m_map.insert( std::make_pair( Keyboard::m_W, "W" ) );
		m_map.insert( std::make_pair( Keyboard::m_X, "X" ) );
		m_map.insert( std::make_pair( Keyboard::m_Y, "Y" ) );
		m_map.insert( std::make_pair( Keyboard::m_Z, "Z" ) );
		/*
		m_map.insert( std::make_pair( Keyboard::m_LWIN, "LWIN" ) );
		m_map.insert( std::make_pair( Keyboard::m_RWIN, "RWIN" ) );
		m_map.insert( std::make_pair( Keyboard::m_APPS, "APPS" ) );
		*/
		/*
		* 0x5E : reserved
		*/

//		m_map.insert( std::make_pair( Keyboard::m_SLEEP, "SLEEP" ) );

		m_map.insert( std::make_pair( Keyboard::m_Num0, "Num0" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num1, "Num1" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num2, "Num2" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num3, "Num3" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num4, "Num4" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num5, "Num5" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num6, "Num6" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num7, "Num7" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num8, "Num8" ) );
		m_map.insert( std::make_pair( Keyboard::m_Num9, "Num9" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumMultiply, "NumMultiply" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumAdd, "NumAdd" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumEnter, "NumEnter" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumSubtract, "NumSubtract" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumDecimal, "NumDecimal" ) );
		m_map.insert( std::make_pair( Keyboard::m_NumDivide, "NumDivide" ) );
		m_map.insert( std::make_pair( Keyboard::m_F1, "F1" ) );
		m_map.insert( std::make_pair( Keyboard::m_F2, "F2" ) );
		m_map.insert( std::make_pair( Keyboard::m_F3, "F3" ) );
		m_map.insert( std::make_pair( Keyboard::m_F4, "F4" ) );
		m_map.insert( std::make_pair( Keyboard::m_F5, "F5" ) );
		m_map.insert( std::make_pair( Keyboard::m_F6, "F6" ) );
		m_map.insert( std::make_pair( Keyboard::m_F7, "F7" ) );
		m_map.insert( std::make_pair( Keyboard::m_F8, "F8" ) );
		m_map.insert( std::make_pair( Keyboard::m_F9, "F9" ) );
		m_map.insert( std::make_pair( Keyboard::m_F10, "F10" ) );
		m_map.insert( std::make_pair( Keyboard::m_F11, "F11" ) );
		m_map.insert( std::make_pair( Keyboard::m_F12, "F12" ) );
		m_map.insert( std::make_pair( Keyboard::m_F13, "F13" ) );
		m_map.insert( std::make_pair( Keyboard::m_F14, "F14" ) );
		m_map.insert( std::make_pair( Keyboard::m_F15, "F15" ) );
		m_map.insert( std::make_pair( Keyboard::m_F16, "F16" ) );
		m_map.insert( std::make_pair( Keyboard::m_F17, "F17" ) );
		m_map.insert( std::make_pair( Keyboard::m_F18, "F18" ) );
		m_map.insert( std::make_pair( Keyboard::m_F19, "F19" ) );
		m_map.insert( std::make_pair( Keyboard::m_F20, "F20" ) );
		m_map.insert( std::make_pair( Keyboard::m_F21, "F21" ) );
		m_map.insert( std::make_pair( Keyboard::m_F22, "F22" ) );
		m_map.insert( std::make_pair( Keyboard::m_F23, "F23" ) );
		m_map.insert( std::make_pair( Keyboard::m_F24, "F24" ) );

		/*
		* 0x88 - 0x8F : unassigned
		*/

		m_map.insert( std::make_pair( Keyboard::m_NumLock, "NumLock" ) );
		m_map.insert( std::make_pair( Keyboard::m_ScrollLock, "ScrollLock" ) );

		/*
		* NEC PC-9800 kbd definitions
		*/

		//		Key_OEM_NEC_EQUAL= 0x92,   // '=' key on numpad

		/*
		* Fujitsu/OASYS kbd definitions
		*/
		/*
		Key_OEM_FJ_JISHO = 0x92,   // 'Dictionary' key
		Key_OEM_FJ_MASSHOU=0x93,   // 'Unregister word' key
		Key_OEM_FJ_TOUROKU=0x94,   // 'Register word' key
		Key_OEM_FJ_LOYA  = 0x95,   // 'Left OYAYUBI' key
		Key_OEM_FJ_ROYA  = 0x96,   // 'Right OYAYUBI' key
		*/
		/*
		* 0x97 - 0x9F : unassigned
		*/

		/*
		* Key_L* & Key_R* - left and right Alt, Ctrl and Shift virtual keys.
		* Used only as parameters to GetAsyncKeyState() and GetKeyState().
		* No other API or message will distinguish left and right keys in this way.
		*/

		m_map.insert( std::make_pair( Keyboard::m_LShift, "LShift" ) );
		m_map.insert( std::make_pair( Keyboard::m_RShift, "RShift" ) );
		m_map.insert( std::make_pair( Keyboard::m_LControl, "LControl" ) );
		m_map.insert( std::make_pair( Keyboard::m_RControl, "RControl" ) );
		m_map.insert( std::make_pair( Keyboard::m_LAlt, "LAlt" ) );
		m_map.insert( std::make_pair( Keyboard::m_RAlt, "RAlt" ) );
		/*
		Key_BROWSER_BACK      = 0xA6,
		Key_BROWSER_FORWARD   = 0xA7,
		Key_BROWSER_REFRESH   = 0xA8,
		Key_BROWSER_STOP      = 0xA9,
		Key_BROWSER_SEARCH    = 0xAA,
		Key_BROWSER_FAVORITES = 0xAB,
		Key_BROWSER_HOME      = 0xAC,

		Key_VOLUME_MUTE       = 0xAD,
		Key_VOLUME_DOWN       = 0xAE,
		Key_VOLUME_UP         = 0xAF,
		Key_MEDIA_NEXT_TRACK  = 0xB0,
		Key_MEDIA_PREV_TRACK  = 0xB1,
		Key_MEDIA_STOP        = 0xB2,
		Key_MEDIA_PLAY_PAUSE  = 0xB3,
		Key_LAUNCH_MAIL       = 0xB4,
		Key_LAUNCH_MEDIA_SELECT 0xB5,
		Key_LAUNCH_APP1       = 0xB6,
		Key_LAUNCH_APP2       = 0xB7,
		*/

		/*
		* 0xB8 - 0xB9 : reserved
		*/

		m_map.insert( std::make_pair( Keyboard::m_Semicolon, "Semicolon" ) );
		m_map.insert( std::make_pair( Keyboard::m_Plus, "Plus" ) );
		m_map.insert( std::make_pair( Keyboard::m_Comma, "Comma" ) );
		m_map.insert( std::make_pair( Keyboard::m_Minus, "Minus" ) );
		m_map.insert( std::make_pair( Keyboard::m_Period, "Period" ) );
		m_map.insert( std::make_pair( Keyboard::m_Question, "Question" ) );
		m_map.insert( std::make_pair( Keyboard::m_Tilde, "Tilde" ) );

		/*
		* 0xC1 - 0xD7 : reserved
		*/

		/*
		* 0xD8 - 0xDA : unassigned
		*/
		/*
		Key_OEM_4        = 0xDB,  //  '[{' for US
		Key_OEM_5        = 0xDC,  //  '\|' for US
		Key_OEM_6        = 0xDD,  //  ']}' for US
		Key_OEM_7        = 0xDE,  //  ''"' for US
		Key_OEM_8        = 0xDF,
		*/
		/*
		* 0xE0 : reserved
		*/

		/*
		* Various extended or enhanced keyboards
		*/
		/*
		Key_OEM_AX       = 0xE1,  //  'AX' key on Japanese AX kbd
		Key_OEM_102      = 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
		Key_ICO_HELP     = 0xE3,  //  Help key on ICO
		Key_ICO_00       = 0xE4,  //  00 key on ICO

		m_map.insert( std::make_pair( Keyboard::_PROCESSKEY, "PROCESSKEY" ) );

		Key_ICO_CLEAR    = 0xE6,

		m_map.insert( std::make_pair( Keyboard::_PACKET, "PACKET" ) );
		*/
		/*
		* 0xE8 : unassigned
		*/

		/*
		* Nokia/Ericsson definitions
		*/
		/*
		Key_OEM_RESET    = 0xE9,
		Key_OEM_JUMP     = 0xEA,
		Key_OEM_PA1      = 0xEB,
		Key_OEM_PA2      = 0xEC,
		Key_OEM_PA3      = 0xED,
		Key_OEM_WSCTRL   = 0xEE,
		Key_OEM_CUSEL    = 0xEF,
		Key_OEM_ATTN     = 0xF0,
		Key_OEM_FINISH   = 0xF1,
		Key_OEM_COPY     = 0xF2,
		Key_OEM_AUTO     = 0xF3,
		Key_OEM_ENLW     = 0xF4,
		Key_OEM_BACKTAB  = 0xF5,

		m_map.insert( std::make_pair( Keyboard::_ATTN, "ATTN" ) );
		m_map.insert( std::make_pair( Keyboard::_CRSEL, "CRSEL" ) );
		m_map.insert( std::make_pair( Keyboard::_EXSEL, "EXSEL" ) );
		m_map.insert( std::make_pair( Keyboard::_EREOF, "EREOF" ) );
		m_map.insert( std::make_pair( Keyboard::_PLAY, "PLAY" ) );
		m_map.insert( std::make_pair( Keyboard::_ZOOM, "ZOOM" ) );
		m_map.insert( std::make_pair( Keyboard::_NONAME, "NONAME" ) );
		m_map.insert( std::make_pair( Keyboard::_PA1, "PA1" ) );
		Key_OEM_CLEAR    = 0xFE,
		*/
		/*
		* 0xFF : reserved
		*/
	}
}