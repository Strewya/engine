#ifndef SUBSYSTEMS_INPUT_KEYCODES_H_
#define SUBSYSTEMS_INPUT_KEYCODES_H_
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
		static unsigned int Code(const std::string& name);
		static std::string Name(const unsigned int &code);

		static const unsigned int _Unknown		= 0x00;
		
		static const unsigned int _LeftButton	= 0x01;
		static const unsigned int _RightButton	= 0x02;
		static const unsigned int _MiddleButton	= 0x04;
		static const unsigned int _XButton1		= 0x05;
		static const unsigned int _XButton2		= 0x06;
	};

	class Key
	{
		typedef std::unordered_map<unsigned int, std::string> Map;
		static Map _map;

		friend void InitializeInputConstants();
		static void Init();
	public:
		static unsigned int Code(const std::string& name);
		static std::string Name(const unsigned int &code);
		
		//0x00 is unasigned, but added here for empty value
		static const unsigned int _Unknown		= 0x00;
		static const unsigned int _Cancel		= 0x03;
	
		/*
		 * 0x07 : unassigned
		 */
		
		static const unsigned int _Backspace	= 0x08;
		static const unsigned int _Tab			= 0x09;
		/*
		0x0A - 0x0B : reserved
		*/
		static const unsigned int _Clear		= 0x0C;
		static const unsigned int _Enter		= 0x0D;

		static const unsigned int _Shift		= 0x10;
		static const unsigned int _Control		= 0x11;
		static const unsigned int _Alt			= 0x12;
		static const unsigned int _Pause		= 0x13;
		static const unsigned int _CapsLock		= 0x14;
		/*
		static const unsigned int _KANA			= 0x15;
		static const unsigned int _HANGEUL		= 0x15;
		static const unsigned int _HANGUL		= 0x15;
		static const unsigned int _JUNJA		= 0x17;
		static const unsigned int _FINAL		= 0x18;
		static const unsigned int _HANJA		= 0x19;
		static const unsigned int _KANJI		= 0x19;
		*/
		static const unsigned int _Escape		= 0x1B;
		/*
		static const unsigned int _CONVERT		= 0x1C;
		static const unsigned int _NONCONVERT	= 0x1D;
		static const unsigned int _ACCEPT		= 0x1E;
		static const unsigned int _MODECHANGE	= 0x1F;
		*/
		static const unsigned int _Space		= 0x20;
		static const unsigned int _PageUp		= 0x21;
		static const unsigned int _PageDown		= 0x22;
		static const unsigned int _End			= 0x23;
		static const unsigned int _Home			= 0x24;
		static const unsigned int _ArrowLeft	= 0x25;
		static const unsigned int _ArrowUp		= 0x26;
		static const unsigned int _ArrowRight	= 0x27;
		static const unsigned int _ArrowDown	= 0x28;
		/*		
		static const unsigned int _Select		= 0x29;
		static const unsigned int _Print		= 0x2A;
		static const unsigned int _Execute		= 0x2B;
		*/
		static const unsigned int _PrintScreen	= 0x2C;
		static const unsigned int _Insert		= 0x2D;
		static const unsigned int _Delete		= 0x2E;
//		static const unsigned int _HELP			= 0x2F;

		static const unsigned int _0			= 0x30;
		static const unsigned int _1			= 0x31;
		static const unsigned int _2			= 0x32;
		static const unsigned int _3			= 0x33;
		static const unsigned int _4			= 0x34;
		static const unsigned int _5			= 0x35;
		static const unsigned int _6			= 0x36;
		static const unsigned int _7			= 0x37;
		static const unsigned int _8			= 0x38;
		static const unsigned int _9			= 0x39;

		static const unsigned int _A			= 0x41;
		static const unsigned int _B			= 0x42;
		static const unsigned int _C			= 0x43;
		static const unsigned int _D			= 0x44;
		static const unsigned int _E			= 0x45;
		static const unsigned int _F			= 0x46;
		static const unsigned int _G			= 0x47;
		static const unsigned int _H			= 0x48;
		static const unsigned int _I			= 0x49;
		static const unsigned int _J			= 0x4A;
		static const unsigned int _K			= 0x4B;
		static const unsigned int _L			= 0x4C;
		static const unsigned int _M			= 0x4D;
		static const unsigned int _N			= 0x4E;
		static const unsigned int _O			= 0x4F;
		static const unsigned int _P			= 0x50;
		static const unsigned int _Q			= 0x51;
		static const unsigned int _R			= 0x52;
		static const unsigned int _S			= 0x53;
		static const unsigned int _T			= 0x54;
		static const unsigned int _U			= 0x55;
		static const unsigned int _V			= 0x56;
		static const unsigned int _W			= 0x57;
		static const unsigned int _X			= 0x58;
		static const unsigned int _Y			= 0x59;
		static const unsigned int _Z			= 0x5A;
		/*
		static const unsigned int _LWIN			= 0x5B;
		static const unsigned int _RWIN			= 0x5C;
		static const unsigned int _APPS			= 0x5D;
		*/
		/*
		* 0x5E : reserved
		*/
//		static const unsigned int _SLEEP		= 0x5F;

		static const unsigned int _Num0			= 0x60;
		static const unsigned int _Num1			= 0x61;
		static const unsigned int _Num2			= 0x62;
		static const unsigned int _Num3			= 0x63;
		static const unsigned int _Num4			= 0x64;
		static const unsigned int _Num5			= 0x65;
		static const unsigned int _Num6			= 0x66;
		static const unsigned int _Num7			= 0x67;
		static const unsigned int _Num8			= 0x68;
		static const unsigned int _Num9			= 0x69;
		static const unsigned int _NumMultiply	= 0x6A;
		static const unsigned int _NumAdd		= 0x6B;
		static const unsigned int _NumEnter		= 0x6C;
		static const unsigned int _NumSubtract	= 0x6D;
		static const unsigned int _NumDecimal	= 0x6E;
		static const unsigned int _NumDivide	= 0x6F;
		static const unsigned int _F1			= 0x70;
		static const unsigned int _F2			= 0x71;
		static const unsigned int _F3			= 0x72;
		static const unsigned int _F4			= 0x73;
		static const unsigned int _F5			= 0x74;
		static const unsigned int _F6			= 0x75;
		static const unsigned int _F7			= 0x76;
		static const unsigned int _F8			= 0x77;
		static const unsigned int _F9			= 0x78;
		static const unsigned int _F10			= 0x79;
		static const unsigned int _F11			= 0x7A;
		static const unsigned int _F12			= 0x7B;
		static const unsigned int _F13			= 0x7C;
		static const unsigned int _F14			= 0x7D;
		static const unsigned int _F15			= 0x7E;
		static const unsigned int _F16			= 0x7F;
		static const unsigned int _F17			= 0x80;
		static const unsigned int _F18			= 0x81;
		static const unsigned int _F19			= 0x82;
		static const unsigned int _F20			= 0x83;
		static const unsigned int _F21			= 0x84;
		static const unsigned int _F22			= 0x85;
		static const unsigned int _F23			= 0x86;
		static const unsigned int _F24			= 0x87;
		/*
		* 0x88 - 0x8F : unassigned
		*/
		static const unsigned int _NumLock		= 0x90;
		static const unsigned int _ScrollLock	= 0x91;
		/*
		* NEC PC-9800 kbd definitions
		*/
//		static const unsigned int _OEM_NEC_EQUAL= 0x92;
		/*
		* Fujitsu/OASYS kbd definitions
		*/
		/*
		static const unsigned int _OEM_FJ_JISHO = 0x92;   // 'Dictionary' key
		static const unsigned int _OEM_FJ_MASSHOU=0x93,   // 'Unregister word' key
		static const unsigned int _OEM_FJ_TOUROKU=0x94,   // 'Register word' key
		static const unsigned int _OEM_FJ_LOYA  = 0x95,   // 'Left OYAYUBI' key
		static const unsigned int _OEM_FJ_ROYA  = 0x96,   // 'Right OYAYUBI' key
		*/
		/*
		* 0x97 - 0x9F : unassigned
		*/
		/*
		* Key_L* & Key_R* - left and right Alt, Ctrl and Shift virtual keys.
		* Used only as parameters to GetAsyncKeyState() and GetKeyState().
		* No other API or message will distinguish left and right keys in this way.
		*/
		static const unsigned int _LShift		= 0xA0;
		static const unsigned int _RShift		= 0xA1;
		static const unsigned int _LControl		= 0xA2;
		static const unsigned int _RControl		= 0xA3;
		static const unsigned int _LAlt			= 0xA4;
		static const unsigned int _RAlt			= 0xA5;
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
		static const unsigned int _Semicolon	= 0xBA;
		static const unsigned int _Plus			= 0xBB;
		static const unsigned int _Comma		= 0xBC;
		static const unsigned int _Minus		= 0xBD;
		static const unsigned int _Period		= 0xBE;
		static const unsigned int _Question		= 0xBF;
		static const unsigned int _Tilde		= 0xC0;
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

		static const unsigned int _PROCESSKEY			= 0xE5;

		Key_ICO_CLEAR    = 0xE6,

		static const unsigned int _PACKET			= 0xE7;
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

		static const unsigned int _ATTN			= 0xF6;
		static const unsigned int _CRSEL			= 0xF7;
		static const unsigned int _EXSEL			= 0xF8;
		static const unsigned int _EREOF			= 0xF9;
		static const unsigned int _PLAY			= 0xFA;
		static const unsigned int _ZOOM			= 0xFB;
		static const unsigned int _NONAME			= 0xFC;
		static const unsigned int _PA1			= 0xFD;
		Key_OEM_CLEAR    = 0xFE,
		*/
		/*
		* 0xFF : reserved
		*/
	};
}

#endif //SUBSYSTEMS_INPUT_KEYCODES_H_