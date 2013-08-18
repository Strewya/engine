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
			_LeftButton		= 0x00,
			_RightButton,
			_MiddleButton,
			_XButton1,
			_XButton2,

			_KeyCount
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
			_Cancel			= 0x03,
	
			/*
			 * 0x07 : unassigned
			 */
		
			_Backspace		= 0x08,
			_Tab			= 0x09,
			/*
			0x0A - 0x0B : reserved
			*/
			_Clear			= 0x0C,
			_Enter			= 0x0D,

			_Shift			= 0x10,
			_Control		= 0x11,
			_Alt			= 0x12,
			_Pause			= 0x13,
			_CapsLock		= 0x14,
			/*
			_KANA			= 0x15,
			_HANGEUL		= 0x15,
			_HANGUL			= 0x15,
			_JUNJA			= 0x17,
			_FINAL			= 0x18,
			_HANJA			= 0x19,
			_KANJI			= 0x19,
			*/
			_Escape			= 0x1B,
			/*
			_CONVERT		= 0x1C,
			_NONCONVERT		= 0x1D,
			_ACCEPT			= 0x1E,
			_MODECHANGE		= 0x1F,
			*/
			_Space			= 0x20,
			_PageUp			= 0x21,
			_PageDown		= 0x22,
			_End			= 0x23,
			_Home			= 0x24,
			_ArrowLeft		= 0x25,
			_ArrowUp		= 0x26,
			_ArrowRight		= 0x27,
			_ArrowDown		= 0x28,
			/*		
			_Select			= 0x29,
			_Print			= 0x2A,
			_Execute		= 0x2B,
			*/
			_PrintScreen	= 0x2C,
			_Insert			= 0x2D,
			_Delete			= 0x2E,
	//		_HELP			= 0x2F,

			_0				= 0x30,
			_1				= 0x31,
			_2				= 0x32,
			_3				= 0x33,
			_4				= 0x34,
			_5				= 0x35,
			_6				= 0x36,
			_7				= 0x37,
			_8				= 0x38,
			_9				= 0x39,

			_A				= 0x41,
			_B				= 0x42,
			_C				= 0x43,
			_D				= 0x44,
			_E				= 0x45,
			_F				= 0x46,
			_G				= 0x47,
			_H				= 0x48,
			_I				= 0x49,
			_J				= 0x4A,
			_K				= 0x4B,
			_L				= 0x4C,
			_M				= 0x4D,
			_N				= 0x4E,
			_O				= 0x4F,
			_P				= 0x50,
			_Q				= 0x51,
			_R				= 0x52,
			_S				= 0x53,
			_T				= 0x54,
			_U				= 0x55,
			_V				= 0x56,
			_W				= 0x57,
			_X				= 0x58,
			_Y				= 0x59,
			_Z				= 0x5A,
			/*		
			_LWIN			= 0x5B,
			_RWIN			= 0x5C,
			_APPS			= 0x5D,
			*/
			/*
			* 0x5E : reserved
			*/
	//		_SLEEP			= 0x5F,

			_Num0			= 0x60,
			_Num1			= 0x61,
			_Num2			= 0x62,
			_Num3			= 0x63,
			_Num4			= 0x64,
			_Num5			= 0x65,
			_Num6			= 0x66,
			_Num7			= 0x67,
			_Num8			= 0x68,
			_Num9			= 0x69,
			_NumMultiply	= 0x6A,
			_NumAdd			= 0x6B,
			_NumEnter		= 0x6C,
			_NumSubtract	= 0x6D,
			_NumDecimal		= 0x6E,
			_NumDivide		= 0x6F,
			_F1				= 0x70,
			_F2				= 0x71,
			_F3				= 0x72,
			_F4				= 0x73,
			_F5				= 0x74,
			_F6				= 0x75,
			_F7				= 0x76,
			_F8				= 0x77,
			_F9				= 0x78,
			_F10			= 0x79,
			_F11			= 0x7A,
			_F12			= 0x7B,
			_F13			= 0x7C,
			_F14			= 0x7D,
			_F15			= 0x7E,
			_F16			= 0x7F,
			_F17			= 0x80,
			_F18			= 0x81,
			_F19			= 0x82,
			_F20			= 0x83,
			_F21			= 0x84,
			_F22			= 0x85,
			_F23			= 0x86,
			_F24			= 0x87,
			/*
			* 0x88 - 0x8F : unassigned
			*/
			_NumLock		= 0x90,
			_ScrollLock		= 0x91,
			/*
			* NEC PC-9800 kbd definitions
			*/
	//		_OEM_NEC_EQUAL	= 0x92,
			/*
			* Fujitsu/OASYS kbd definitions
			*/
			/*
			_OEM_FJ_JISHO	= 0x92,   // 'Dictionary' key
			_OEM_FJ_MASSHOU	= 0x93,   // 'Unregister word' key
			_OEM_FJ_TOUROKU	= 0x94,   // 'Register word' key
			_OEM_FJ_LOYA	= 0x95,   // 'Left OYAYUBI' key
			_OEM_FJ_ROYA	= 0x96,   // 'Right OYAYUBI' key
			*/
			/*
			* 0x97 - 0x9F : unassigned
			*/
			/*
			* Key_L* & Key_R* - left and right Alt, Ctrl and Shift virtual keys.
			* Used only as parameters to GetAsyncKeyState() and GetKeyState().
			* No other API or message will distinguish left and right keys in this way.
			*/
			_LShift			= 0xA0,
			_RShift			= 0xA1,
			_LControl		= 0xA2,
			_RControl		= 0xA3,
			_LAlt			= 0xA4,
			_RAlt			= 0xA5,
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
			_Semicolon		= 0xBA,
			_Plus			= 0xBB,
			_Comma			= 0xBC,
			_Minus			= 0xBD,
			_Period			= 0xBE,
			_Question		= 0xBF,
			_Tilde			= 0xC0,
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

			_KeyCount		= 0x100
		};
	};
}