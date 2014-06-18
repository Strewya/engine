//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Input/Service/KeyCodes.h>
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/



namespace Input
{
	Keyboard::Map Keyboard::_map;
	Mouse::Map Mouse::_map;

	void InitializeInputConstants()
	{
		Keyboard::Init();
		Mouse::Init();
	}

	int Mouse::Code(const std::string& name)
	{
		Map::iterator it = _map.begin();
		Map::iterator end = _map.end();
		for(; it != end; ++it)
		{
			if(it->second == name)
				return it->first;
		}
		return _Unknown;
	}

	std::string Mouse::Name(const unsigned int& code)
	{
		Map::iterator it = _map.find(code);
		return (it != _map.end() ? it->second : "" );
	}
	
	void Mouse::Init()
	{
		_map.insert( std::make_pair( Mouse::m_LeftButton, "LeftButton" ) );
		_map.insert( std::make_pair( Mouse::m_RightButton, "RightButton" ) );
		_map.insert( std::make_pair( Mouse::m_MiddleButton, "MiddleButton" ) );
		_map.insert( std::make_pair( Mouse::m_XButton1, "XButton1" ) );
		_map.insert( std::make_pair( Mouse::m_XButton2, "XButton2" ) );
	}
	
	int Keyboard::Code(const std::string& name)
	{
		Map::iterator it = _map.begin();
		Map::iterator end = _map.end();
		for(; it != end; ++it)
		{
			if(it->second == name)
				return it->first;
		}
		return _Unknown;
	}

	std::string Keyboard::Name(const unsigned int& code)
	{
		Map::iterator it = _map.find(code);
		return (it != _map.end() ? it->second : "" );
	}
	
	void Keyboard::Init()
	{
		_map.insert( std::make_pair( Keyboard::m_Cancel, "Cancel" ) );
		
		/*
		* 0x07 : unassigned
		*/

		_map.insert( std::make_pair( Keyboard::m_Backspace, "Backspace" ) );
		_map.insert( std::make_pair( Keyboard::m_Tab, "Tab" ) );

		/*
		* 0x0A - 0x0B : reserved
		*/

		_map.insert( std::make_pair( Keyboard::m_Clear, "Clear" ) );
		_map.insert( std::make_pair( Keyboard::m_Enter, "Enter" ) );

		_map.insert( std::make_pair( Keyboard::m_Shift, "Shift" ) );
		_map.insert( std::make_pair( Keyboard::m_Control, "Control" ) );
		_map.insert( std::make_pair( Keyboard::m_Alt, "Alt" ) );
		_map.insert( std::make_pair( Keyboard::m_Pause, "Pause" ) );
		_map.insert( std::make_pair( Keyboard::m_CapsLock, "CapsLock" ) );
		/*
		_map.insert( std::make_pair( Keyboard::m_KANA, "KANA" ) );
		_map.insert( std::make_pair( Keyboard::m_HANGEUL, "HANGEUL" ) );
		_map.insert( std::make_pair( Keyboard::m_HANGUL, "HANGUL" ) );
		_map.insert( std::make_pair( Keyboard::m_JUNJA, "JUNJA" ) );
		_map.insert( std::make_pair( Keyboard::m_FINAL, "FINAL" ) );
		_map.insert( std::make_pair( Keyboard::m_HANJA, "HANJA" ) );
		_map.insert( std::make_pair( Keyboard::m_KANJI, "KANJI" ) );
		*/
		_map.insert( std::make_pair( Keyboard::m_Escape, "Escape" ) );
		/*
		_map.insert( std::make_pair( Keyboard::m_CONVERT, "CONVERT" ) );
		_map.insert( std::make_pair( Keyboard::m_NONCONVERT, "NONCONVERT" ) );
		_map.insert( std::make_pair( Keyboard::m_ACCEPT, "ACCEPT" ) );
		_map.insert( std::make_pair( Keyboard::m_MODECHANGE, "MODECHANGE" ) );
		*/
		_map.insert( std::make_pair( Keyboard::m_Space, "Space" ) );
		_map.insert( std::make_pair( Keyboard::m_PageUp, "PageUp" ) );
		_map.insert( std::make_pair( Keyboard::m_PageDown, "PageDown" ) );
		_map.insert( std::make_pair( Keyboard::m_End, "End" ) );
		_map.insert( std::make_pair( Keyboard::m_Home, "Home" ) );
		_map.insert( std::make_pair( Keyboard::m_ArrowLeft, "ArrowLeft" ) );
		_map.insert( std::make_pair( Keyboard::m_ArrowUp, "ArrowUp" ) );
		_map.insert( std::make_pair( Keyboard::m_ArrowRight, "ArrowRight" ) );
		_map.insert( std::make_pair( Keyboard::m_ArrowDown, "ArrowDown" ) );
//		_map.insert( std::make_pair( Keyboard::m_Select, "Select" ) );
//		_map.insert( std::make_pair( Keyboard::m_Print, "Print" ) );
//		_map.insert( std::make_pair( Keyboard::m_Execute, "Execute" ) );
		_map.insert( std::make_pair( Keyboard::m_PrintScreen, "PrintScreen" ) );
		_map.insert( std::make_pair( Keyboard::m_Insert, "Insert" ) );
		_map.insert( std::make_pair( Keyboard::m_Delete, "Delete" ) );
//		_map.insert( std::make_pair( Keyboard::m_HELP, "HELP" ) );

		_map.insert( std::make_pair( Keyboard::m_0, "0" ) );
		_map.insert( std::make_pair( Keyboard::m_1, "1" ) );
		_map.insert( std::make_pair( Keyboard::m_2, "2" ) );
		_map.insert( std::make_pair( Keyboard::m_3, "3" ) );
		_map.insert( std::make_pair( Keyboard::m_4, "4" ) );
		_map.insert( std::make_pair( Keyboard::m_5, "5" ) );
		_map.insert( std::make_pair( Keyboard::m_6, "6" ) );
		_map.insert( std::make_pair( Keyboard::m_7, "7" ) );
		_map.insert( std::make_pair( Keyboard::m_8, "8" ) );
		_map.insert( std::make_pair( Keyboard::m_9, "9" ) );

		_map.insert( std::make_pair( Keyboard::m_A, "A" ) );
		_map.insert( std::make_pair( Keyboard::m_B, "B" ) );
		_map.insert( std::make_pair( Keyboard::m_C, "C" ) );
		_map.insert( std::make_pair( Keyboard::m_D, "D" ) );
		_map.insert( std::make_pair( Keyboard::m_E, "E" ) );
		_map.insert( std::make_pair( Keyboard::m_F, "F" ) );
		_map.insert( std::make_pair( Keyboard::m_G, "G" ) );
		_map.insert( std::make_pair( Keyboard::m_H, "H" ) );
		_map.insert( std::make_pair( Keyboard::m_I, "I" ) );
		_map.insert( std::make_pair( Keyboard::m_J, "J" ) );
		_map.insert( std::make_pair( Keyboard::m_K, "K" ) );
		_map.insert( std::make_pair( Keyboard::m_L, "L" ) );
		_map.insert( std::make_pair( Keyboard::m_M, "M" ) );
		_map.insert( std::make_pair( Keyboard::m_N, "N" ) );
		_map.insert( std::make_pair( Keyboard::m_O, "O" ) );
		_map.insert( std::make_pair( Keyboard::m_P, "P" ) );
		_map.insert( std::make_pair( Keyboard::m_Q, "Q" ) );
		_map.insert( std::make_pair( Keyboard::m_R, "R" ) );
		_map.insert( std::make_pair( Keyboard::m_S, "S" ) );
		_map.insert( std::make_pair( Keyboard::m_T, "T" ) );
		_map.insert( std::make_pair( Keyboard::m_U, "U" ) );
		_map.insert( std::make_pair( Keyboard::m_V, "V" ) );
		_map.insert( std::make_pair( Keyboard::m_W, "W" ) );
		_map.insert( std::make_pair( Keyboard::m_X, "X" ) );
		_map.insert( std::make_pair( Keyboard::m_Y, "Y" ) );
		_map.insert( std::make_pair( Keyboard::m_Z, "Z" ) );
		/*
		_map.insert( std::make_pair( Keyboard::m_LWIN, "LWIN" ) );
		_map.insert( std::make_pair( Keyboard::m_RWIN, "RWIN" ) );
		_map.insert( std::make_pair( Keyboard::m_APPS, "APPS" ) );
		*/
		/*
		* 0x5E : reserved
		*/

//		_map.insert( std::make_pair( Keyboard::m_SLEEP, "SLEEP" ) );

		_map.insert( std::make_pair( Keyboard::m_Num0, "Num0" ) );
		_map.insert( std::make_pair( Keyboard::m_Num1, "Num1" ) );
		_map.insert( std::make_pair( Keyboard::m_Num2, "Num2" ) );
		_map.insert( std::make_pair( Keyboard::m_Num3, "Num3" ) );
		_map.insert( std::make_pair( Keyboard::m_Num4, "Num4" ) );
		_map.insert( std::make_pair( Keyboard::m_Num5, "Num5" ) );
		_map.insert( std::make_pair( Keyboard::m_Num6, "Num6" ) );
		_map.insert( std::make_pair( Keyboard::m_Num7, "Num7" ) );
		_map.insert( std::make_pair( Keyboard::m_Num8, "Num8" ) );
		_map.insert( std::make_pair( Keyboard::m_Num9, "Num9" ) );
		_map.insert( std::make_pair( Keyboard::m_NumMultiply, "NumMultiply" ) );
		_map.insert( std::make_pair( Keyboard::m_NumAdd, "NumAdd" ) );
		_map.insert( std::make_pair( Keyboard::m_NumEnter, "NumEnter" ) );
		_map.insert( std::make_pair( Keyboard::m_NumSubtract, "NumSubtract" ) );
		_map.insert( std::make_pair( Keyboard::m_NumDecimal, "NumDecimal" ) );
		_map.insert( std::make_pair( Keyboard::m_NumDivide, "NumDivide" ) );
		_map.insert( std::make_pair( Keyboard::m_F1, "F1" ) );
		_map.insert( std::make_pair( Keyboard::m_F2, "F2" ) );
		_map.insert( std::make_pair( Keyboard::m_F3, "F3" ) );
		_map.insert( std::make_pair( Keyboard::m_F4, "F4" ) );
		_map.insert( std::make_pair( Keyboard::m_F5, "F5" ) );
		_map.insert( std::make_pair( Keyboard::m_F6, "F6" ) );
		_map.insert( std::make_pair( Keyboard::m_F7, "F7" ) );
		_map.insert( std::make_pair( Keyboard::m_F8, "F8" ) );
		_map.insert( std::make_pair( Keyboard::m_F9, "F9" ) );
		_map.insert( std::make_pair( Keyboard::m_F10, "F10" ) );
		_map.insert( std::make_pair( Keyboard::m_F11, "F11" ) );
		_map.insert( std::make_pair( Keyboard::m_F12, "F12" ) );
		_map.insert( std::make_pair( Keyboard::m_F13, "F13" ) );
		_map.insert( std::make_pair( Keyboard::m_F14, "F14" ) );
		_map.insert( std::make_pair( Keyboard::m_F15, "F15" ) );
		_map.insert( std::make_pair( Keyboard::m_F16, "F16" ) );
		_map.insert( std::make_pair( Keyboard::m_F17, "F17" ) );
		_map.insert( std::make_pair( Keyboard::m_F18, "F18" ) );
		_map.insert( std::make_pair( Keyboard::m_F19, "F19" ) );
		_map.insert( std::make_pair( Keyboard::m_F20, "F20" ) );
		_map.insert( std::make_pair( Keyboard::m_F21, "F21" ) );
		_map.insert( std::make_pair( Keyboard::m_F22, "F22" ) );
		_map.insert( std::make_pair( Keyboard::m_F23, "F23" ) );
		_map.insert( std::make_pair( Keyboard::m_F24, "F24" ) );

		/*
		* 0x88 - 0x8F : unassigned
		*/

		_map.insert( std::make_pair( Keyboard::m_NumLock, "NumLock" ) );
		_map.insert( std::make_pair( Keyboard::m_ScrollLock, "ScrollLock" ) );

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

		_map.insert( std::make_pair( Keyboard::m_LShift, "LShift" ) );
		_map.insert( std::make_pair( Keyboard::m_RShift, "RShift" ) );
		_map.insert( std::make_pair( Keyboard::m_LControl, "LControl" ) );
		_map.insert( std::make_pair( Keyboard::m_RControl, "RControl" ) );
		_map.insert( std::make_pair( Keyboard::m_LAlt, "LAlt" ) );
		_map.insert( std::make_pair( Keyboard::m_RAlt, "RAlt" ) );
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

		_map.insert( std::make_pair( Keyboard::m_Semicolon, "Semicolon" ) );
		_map.insert( std::make_pair( Keyboard::m_Plus, "Plus" ) );
		_map.insert( std::make_pair( Keyboard::m_Comma, "Comma" ) );
		_map.insert( std::make_pair( Keyboard::m_Minus, "Minus" ) );
		_map.insert( std::make_pair( Keyboard::m_Period, "Period" ) );
		_map.insert( std::make_pair( Keyboard::m_Question, "Question" ) );
		_map.insert( std::make_pair( Keyboard::m_Tilde, "Tilde" ) );

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

		_map.insert( std::make_pair( Keyboard::_PROCESSKEY, "PROCESSKEY" ) );

		Key_ICO_CLEAR    = 0xE6,

		_map.insert( std::make_pair( Keyboard::_PACKET, "PACKET" ) );
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

		_map.insert( std::make_pair( Keyboard::_ATTN, "ATTN" ) );
		_map.insert( std::make_pair( Keyboard::_CRSEL, "CRSEL" ) );
		_map.insert( std::make_pair( Keyboard::_EXSEL, "EXSEL" ) );
		_map.insert( std::make_pair( Keyboard::_EREOF, "EREOF" ) );
		_map.insert( std::make_pair( Keyboard::_PLAY, "PLAY" ) );
		_map.insert( std::make_pair( Keyboard::_ZOOM, "ZOOM" ) );
		_map.insert( std::make_pair( Keyboard::_NONAME, "NONAME" ) );
		_map.insert( std::make_pair( Keyboard::_PA1, "PA1" ) );
		Key_OEM_CLEAR    = 0xFE,
		*/
		/*
		* 0xFF : reserved
		*/
	}
}