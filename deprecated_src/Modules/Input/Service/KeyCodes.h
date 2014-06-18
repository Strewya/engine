#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <unordered_map>
#include <string>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/


namespace Input
{
	void InitializeInputConstants();

	class Mouse
	{
	private:
		typedef std::unordered_map<unsigned int, std::string> Map;
		static Map _map;

		friend void InitializeInputConstants();
		static void Init();

	public:
		static int Code(const std::string& name);
		static std::string Name(const unsigned int &code);

		static const int _Unknown = -1;
		enum Keys
		{
			m_LeftButton		= 0x00,
			m_RightButton,
			m_MiddleButton,
			m_XButton1,
			m_XButton2,

			m_KeyCount
		};
	};

	class Keyboard
	{
		typedef std::unordered_map<unsigned int, std::string> Map;
		static Map _map;

		friend void InitializeInputConstants();
		static void Init();
	public:
		static int Code(const std::string& name);
		static std::string Name(const unsigned int &code);
		
		static const int _Unknown = -1;
		enum Keys
		{
			m_Cancel		= 0x03,
	
			/*
			 * 0x07 : unassigned
			 */
		
			m_Backspace		= 0x08,
			m_Tab			= 0x09,
			/*
			0x0A - 0x0B : reserved
			*/
			m_Clear			= 0x0C,
			m_Enter			= 0x0D,

			m_Shift			= 0x10,
			m_Control		= 0x11,
			m_Alt			= 0x12,
			m_Pause			= 0x13,
			m_CapsLock		= 0x14,
			/*
			m_KANA			= 0x15,
			m_HANGEUL		= 0x15,
			m_HANGUL		= 0x15,
			m_JUNJA			= 0x17,
			m_FINAL			= 0x18,
			m_HANJA			= 0x19,
			m_KANJI			= 0x19,
			*/
			m_Escape		= 0x1B,
			/*
			m_CONVERT		= 0x1C,
			m_NONCONVERT	= 0x1D,
			m_ACCEPT		= 0x1E,
			m_MODECHANGE	= 0x1F,
			*/
			m_Space			= 0x20,
			m_PageUp		= 0x21,
			m_PageDown		= 0x22,
			m_End			= 0x23,
			m_Home			= 0x24,
			m_ArrowLeft		= 0x25,
			m_ArrowUp		= 0x26,
			m_ArrowRight	= 0x27,
			m_ArrowDown		= 0x28,
			/*		
			m_Select		= 0x29,
			m_Print			= 0x2A,
			m_Execute		= 0x2B,
			*/
			m_PrintScreen	= 0x2C,
			m_Insert		= 0x2D,
			m_Delete		= 0x2E,
	//		m_HELP			= 0x2F,

			m_0				= 0x30,
			m_1				= 0x31,
			m_2				= 0x32,
			m_3				= 0x33,
			m_4				= 0x34,
			m_5				= 0x35,
			m_6				= 0x36,
			m_7				= 0x37,
			m_8				= 0x38,
			m_9				= 0x39,

			m_A				= 0x41,
			m_B				= 0x42,
			m_C				= 0x43,
			m_D				= 0x44,
			m_E				= 0x45,
			m_F				= 0x46,
			m_G				= 0x47,
			m_H				= 0x48,
			m_I				= 0x49,
			m_J				= 0x4A,
			m_K				= 0x4B,
			m_L				= 0x4C,
			m_M				= 0x4D,
			m_N				= 0x4E,
			m_O				= 0x4F,
			m_P				= 0x50,
			m_Q				= 0x51,
			m_R				= 0x52,
			m_S				= 0x53,
			m_T				= 0x54,
			m_U				= 0x55,
			m_V				= 0x56,
			m_W				= 0x57,
			m_X				= 0x58,
			m_Y				= 0x59,
			m_Z				= 0x5A,
			/*		
			m_LWIN			= 0x5B,
			m_RWIN			= 0x5C,
			m_APPS			= 0x5D,
			*/
			/*
			* 0x5E : reserved
			*/
	//		m_SLEEP			= 0x5F,

			m_Num0			= 0x60,
			m_Num1			= 0x61,
			m_Num2			= 0x62,
			m_Num3			= 0x63,
			m_Num4			= 0x64,
			m_Num5			= 0x65,
			m_Num6			= 0x66,
			m_Num7			= 0x67,
			m_Num8			= 0x68,
			m_Num9			= 0x69,
			m_NumMultiply	= 0x6A,
			m_NumAdd		= 0x6B,
			m_NumEnter		= 0x6C,
			m_NumSubtract	= 0x6D,
			m_NumDecimal	= 0x6E,
			m_NumDivide		= 0x6F,
			m_F1			= 0x70,
			m_F2			= 0x71,
			m_F3			= 0x72,
			m_F4			= 0x73,
			m_F5			= 0x74,
			m_F6			= 0x75,
			m_F7			= 0x76,
			m_F8			= 0x77,
			m_F9			= 0x78,
			m_F10			= 0x79,
			m_F11			= 0x7A,
			m_F12			= 0x7B,
			m_F13			= 0x7C,
			m_F14			= 0x7D,
			m_F15			= 0x7E,
			m_F16			= 0x7F,
			m_F17			= 0x80,
			m_F18			= 0x81,
			m_F19			= 0x82,
			m_F20			= 0x83,
			m_F21			= 0x84,
			m_F22			= 0x85,
			m_F23			= 0x86,
			m_F24			= 0x87,
			/*
			* 0x88 - 0x8F : unassigned
			*/
			m_NumLock		= 0x90,
			m_ScrollLock	= 0x91,
			/*
			* NEC PC-9800 kbd definitions
			*/
	//		m_OEM_NEC_EQUAL	= 0x92,
			/*
			* Fujitsu/OASYS kbd definitions
			*/
			/*
			m_OEM_FJ_JISHO	= 0x92,   // 'Dictionary' key
			m_OEM_FJ_MASSHOU= 0x93,   // 'Unregister word' key
			m_OEM_FJ_TOUROKU= 0x94,   // 'Register word' key
			m_OEM_FJ_LOYA	= 0x95,   // 'Left OYAYUBI' key
			m_OEM_FJ_ROYA	= 0x96,   // 'Right OYAYUBI' key
			*/
			/*
			* 0x97 - 0x9F : unassigned
			*/
			/*
			* Key_L* & Key_R* - left and right Alt, Ctrl and Shift virtual keys.
			* Used only as parameters to GetAsyncKeyState() and GetKeyState().
			* No other API or message will distinguish left and right keys in this way.
			*/
			m_LShift		= 0xA0,
			m_RShift		= 0xA1,
			m_LControl		= 0xA2,
			m_RControl		= 0xA3,
			m_LAlt			= 0xA4,
			m_RAlt			= 0xA5,
			/*
			m_BROWSER_BACK		= 0xA6,
			m_BROWSER_FORWARD   = 0xA7,
			m_BROWSER_REFRESH   = 0xA8,
			m_BROWSER_STOP      = 0xA9,
			m_BROWSER_SEARCH    = 0xAA,
			m_BROWSER_FAVORITES = 0xAB,
			m_BROWSER_HOME      = 0xAC,

			m_VOLUME_MUTE       = 0xAD,
			m_VOLUME_DOWN       = 0xAE,
			m_VOLUME_UP         = 0xAF,
			m_MEDIA_NEXT_TRACK  = 0xB0,
			m_MEDIA_PREV_TRACK  = 0xB1,
			m_MEDIA_STOP        = 0xB2,
			m_MEDIA_PLAY_PAUSE  = 0xB3,
			m_LAUNCH_MAIL       = 0xB4,
			m_LAUNCH_MEDIA_SELECT = 0xB5,
			m_LAUNCH_APP1       = 0xB6,
			m_LAUNCH_APP2       = 0xB7,
			*/
			/*
			* 0xB8 - 0xB9 : reserved
			*/
			m_Semicolon		= 0xBA,
			m_Plus			= 0xBB,
			m_Comma			= 0xBC,
			m_Minus			= 0xBD,
			m_Period		= 0xBE,
			m_Question		= 0xBF,
			m_Tilde			= 0xC0,
			/*
			* 0xC1 - 0xD7 : reserved
			*/
			/*
			* 0xD8 - 0xDA : unassigned
			*/
			/*
			Key_OEM_4       = 0xDB,  //  '[{' for US
			Key_OEM_5       = 0xDC,  //  '\|' for US
			Key_OEM_6       = 0xDD,  //  ']}' for US
			Key_OEM_7       = 0xDE,  //  ''"' for US
			Key_OEM_8       = 0xDF,
			*/
			/*
			* 0xE0 : reserved
			*/
			/*
			* Various extended or enhanced keyboards
			*/
			/*
			Key_OEM_AX      = 0xE1,  //  'AX' key on Japanese AX kbd
			Key_OEM_102     = 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
			Key_ICO_HELP    = 0xE3,  //  Help key on ICO
			Key_ICO_00      = 0xE4,  //  00 key on ICO

			_PROCESSKEY		= 0xE5,

			Key_ICO_CLEAR   = 0xE6,

			_PACKET			= 0xE7,
			*/
			/*
			* 0xE8 : unassigned
			*/

			/*
			* Nokia/Ericsson definitions
			*/
			/*
			Key_OEM_RESET   = 0xE9,
			Key_OEM_JUMP    = 0xEA,
			Key_OEM_PA1     = 0xEB,
			Key_OEM_PA2     = 0xEC,
			Key_OEM_PA3     = 0xED,
			Key_OEM_WSCTRL  = 0xEE,
			Key_OEM_CUSEL   = 0xEF,
			Key_OEM_ATTN    = 0xF0,
			Key_OEM_FINISH  = 0xF1,
			Key_OEM_COPY    = 0xF2,
			Key_OEM_AUTO    = 0xF3,
			Key_OEM_ENLW    = 0xF4,
			Key_OEM_BACKTAB = 0xF5,

			_ATTN			= 0xF6,
			_CRSEL			= 0xF7,
			_EXSEL			= 0xF8,
			_EREOF			= 0xF9,
			_PLAY			= 0xFA,
			_ZOOM			= 0xFB,
			_NONAME			= 0xFC,
			_PA1			= 0xFD,
			Key_OEM_CLEAR   = 0xFE,
			*/
			/*
			* 0xFF : reserved
			*/

			m_KeyCount		= 0x100
		};
	};
}