//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/keyboard_handler.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "input/keyboard.h"
#include "utility/communication_buffer.h"
/******* end headers *******/

namespace core
{
   void KeyboardHandler::handle(CommunicationBuffer* buffer, u32 msg, WPARAM wp, LPARAM lp)
   {
      WinMsg we;
      bool matched = false;
      switch( msg )
      {
         case WM_KEYDOWN:
         case WM_KEYUP:
         case WM_SYSKEYDOWN:
         case WM_SYSKEYUP:
         {
            matched = true;
            we.type = WinMsgType::KeyboardKey;
            we.keyboard.key.id = m_keyCodes[(u8)wp];
            we.keyboard.key.isDown = (lp & (1 << 31)) == 0;
            we.keyboard.repeatCount = (u8)LOWORD(lp);
            we.keyboard.firstTimeDown = (lp & (1 << 30)) == 0;
         } break;

         case WM_CHAR:
         {
            matched = true;
            we.type = WinMsgType::KeyboardText;
            we.keyboard.key.id = m_keyCodes[(u8)wp];
            we.keyboard.key.isDown = true;
            we.keyboard.repeatCount = (u8)LOWORD(lp);
            we.keyboard.firstTimeDown = false;
         } break;
      }
      if( matched )
      {
         buffer->writeEvent(we);
      }
   }



   KeyboardHandler::KeyboardHandler()
   {
      memset(m_keyCodes, Keyboard::Unknown, MAX_KEYS);
      m_keyCodes[VK_LBUTTON] = Keyboard::Unknown;
      m_keyCodes[VK_RBUTTON] = Keyboard::Unknown;
      m_keyCodes[VK_CANCEL] = Keyboard::Unknown;
      m_keyCodes[VK_MBUTTON] = Keyboard::Unknown;
      m_keyCodes[VK_XBUTTON1] = Keyboard::Unknown;
      m_keyCodes[VK_XBUTTON2] = Keyboard::Unknown;
      m_keyCodes[VK_BACK] = Keyboard::Backspace;
      m_keyCodes[VK_TAB] = Keyboard::Tab;
      m_keyCodes[VK_CLEAR] = Keyboard::Unknown;
      m_keyCodes[VK_RETURN] = Keyboard::Enter;
      m_keyCodes[VK_SHIFT] = Keyboard::Shift;
      m_keyCodes[VK_CONTROL] = Keyboard::Control;
      m_keyCodes[VK_MENU] = Keyboard::Alt;
      m_keyCodes[VK_PAUSE] = Keyboard::Pause;
      m_keyCodes[VK_CAPITAL] = Keyboard::CapsLock;
      m_keyCodes[VK_KANA] = Keyboard::Unknown;
      m_keyCodes[VK_HANGEUL] = Keyboard::Unknown;
      m_keyCodes[VK_HANGUL] = Keyboard::Unknown;
      m_keyCodes[VK_JUNJA] = Keyboard::Unknown;
      m_keyCodes[VK_FINAL] = Keyboard::Unknown;
      m_keyCodes[VK_HANJA] = Keyboard::Unknown;
      m_keyCodes[VK_KANJI] = Keyboard::Unknown;
      m_keyCodes[VK_ESCAPE] = Keyboard::Escape;
      m_keyCodes[VK_CONVERT] = Keyboard::Unknown;
      m_keyCodes[VK_NONCONVERT] = Keyboard::Unknown;
      m_keyCodes[VK_ACCEPT] = Keyboard::Unknown;
      m_keyCodes[VK_MODECHANGE] = Keyboard::Unknown;
      m_keyCodes[VK_SPACE] = Keyboard::Space;
      m_keyCodes[VK_PRIOR] = Keyboard::PageUp;
      m_keyCodes[VK_NEXT] = Keyboard::PageDown;
      m_keyCodes[VK_END] = Keyboard::End;
      m_keyCodes[VK_HOME] = Keyboard::Home;
      m_keyCodes[VK_LEFT] = Keyboard::ArrowLeft;
      m_keyCodes[VK_UP] = Keyboard::ArrowUp;
      m_keyCodes[VK_RIGHT] = Keyboard::ArrowRight;
      m_keyCodes[VK_DOWN] = Keyboard::ArrowDown;
      m_keyCodes[VK_SELECT] = Keyboard::Unknown;
      m_keyCodes[VK_PRINT] = Keyboard::Unknown;
      m_keyCodes[VK_EXECUTE] = Keyboard::Unknown;
      m_keyCodes[VK_SNAPSHOT] = Keyboard::PrintScreen;
      m_keyCodes[VK_INSERT] = Keyboard::Insert;
      m_keyCodes[VK_DELETE] = Keyboard::Delete;
      m_keyCodes[VK_HELP] = Keyboard::Unknown;
      m_keyCodes['0'] = Keyboard::_0;
      m_keyCodes['1'] = Keyboard::_1;
      m_keyCodes['2'] = Keyboard::_2;
      m_keyCodes['3'] = Keyboard::_3;
      m_keyCodes['4'] = Keyboard::_4;
      m_keyCodes['5'] = Keyboard::_5;
      m_keyCodes['6'] = Keyboard::_6;
      m_keyCodes['7'] = Keyboard::_7;
      m_keyCodes['8'] = Keyboard::_8;
      m_keyCodes['9'] = Keyboard::_9;
      m_keyCodes['A'] = Keyboard::A;
      m_keyCodes['B'] = Keyboard::B;
      m_keyCodes['C'] = Keyboard::C;
      m_keyCodes['D'] = Keyboard::D;
      m_keyCodes['E'] = Keyboard::E;
      m_keyCodes['F'] = Keyboard::F;
      m_keyCodes['G'] = Keyboard::G;
      m_keyCodes['I'] = Keyboard::I;
      m_keyCodes['J'] = Keyboard::J;
      m_keyCodes['K'] = Keyboard::K;
      m_keyCodes['L'] = Keyboard::L;
      m_keyCodes['M'] = Keyboard::M;
      m_keyCodes['N'] = Keyboard::N;
      m_keyCodes['O'] = Keyboard::O;
      m_keyCodes['P'] = Keyboard::P;
      m_keyCodes['Q'] = Keyboard::Q;
      m_keyCodes['R'] = Keyboard::R;
      m_keyCodes['S'] = Keyboard::S;
      m_keyCodes['T'] = Keyboard::T;
      m_keyCodes['U'] = Keyboard::U;
      m_keyCodes['V'] = Keyboard::V;
      m_keyCodes['W'] = Keyboard::W;
      m_keyCodes['X'] = Keyboard::X;
      m_keyCodes['Y'] = Keyboard::Y;
      m_keyCodes['Z'] = Keyboard::Z;
      m_keyCodes[VK_LWIN] = Keyboard::Unknown;
      m_keyCodes[VK_RWIN] = Keyboard::Unknown;
      m_keyCodes[VK_APPS] = Keyboard::Unknown;
      m_keyCodes[VK_SLEEP] = Keyboard::Unknown;
      m_keyCodes[VK_NUMPAD0] = Keyboard::Num0;
      m_keyCodes[VK_NUMPAD1] = Keyboard::Num1;
      m_keyCodes[VK_NUMPAD2] = Keyboard::Num2;
      m_keyCodes[VK_NUMPAD3] = Keyboard::Num3;
      m_keyCodes[VK_NUMPAD4] = Keyboard::Num4;
      m_keyCodes[VK_NUMPAD5] = Keyboard::Num5;
      m_keyCodes[VK_NUMPAD6] = Keyboard::Num6;
      m_keyCodes[VK_NUMPAD7] = Keyboard::Num7;
      m_keyCodes[VK_NUMPAD8] = Keyboard::Num8;
      m_keyCodes[VK_NUMPAD9] = Keyboard::Num9;
      m_keyCodes[VK_MULTIPLY] = Keyboard::NumMultiply;
      m_keyCodes[VK_ADD] = Keyboard::NumAdd;
      m_keyCodes[VK_SEPARATOR] = Keyboard::NumEnter;
      m_keyCodes[VK_SUBTRACT] = Keyboard::NumSubtract;
      m_keyCodes[VK_DECIMAL] = Keyboard::NumDecimal;
      m_keyCodes[VK_DIVIDE] = Keyboard::NumDivide;
      m_keyCodes[VK_F1] = Keyboard::F1;
      m_keyCodes[VK_F2] = Keyboard::F2;
      m_keyCodes[VK_F3] = Keyboard::F3;
      m_keyCodes[VK_F4] = Keyboard::F4;
      m_keyCodes[VK_F5] = Keyboard::F5;
      m_keyCodes[VK_F6] = Keyboard::F6;
      m_keyCodes[VK_F7] = Keyboard::F7;
      m_keyCodes[VK_F8] = Keyboard::F8;
      m_keyCodes[VK_F9] = Keyboard::F9;
      m_keyCodes[VK_F10] = Keyboard::F10;
      m_keyCodes[VK_F11] = Keyboard::F11;
      m_keyCodes[VK_F12] = Keyboard::F12;
      m_keyCodes[VK_F13] = Keyboard::Unknown;
      m_keyCodes[VK_F14] = Keyboard::Unknown;
      m_keyCodes[VK_F15] = Keyboard::Unknown;
      m_keyCodes[VK_F16] = Keyboard::Unknown;
      m_keyCodes[VK_F17] = Keyboard::Unknown;
      m_keyCodes[VK_F18] = Keyboard::Unknown;
      m_keyCodes[VK_F19] = Keyboard::Unknown;
      m_keyCodes[VK_F20] = Keyboard::Unknown;
      m_keyCodes[VK_F21] = Keyboard::Unknown;
      m_keyCodes[VK_F22] = Keyboard::Unknown;
      m_keyCodes[VK_F23] = Keyboard::Unknown;
      m_keyCodes[VK_F24] = Keyboard::Unknown;
      m_keyCodes[VK_NUMLOCK] = Keyboard::NumLock;
      m_keyCodes[VK_SCROLL] = Keyboard::ScrollLock;
      m_keyCodes[VK_OEM_NEC_EQUAL] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FJ_JISHO] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FJ_MASSHOU] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FJ_TOUROKU] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FJ_LOYA] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FJ_ROYA] = Keyboard::Unknown;
      m_keyCodes[VK_LSHIFT] = Keyboard::LShift;
      m_keyCodes[VK_RSHIFT] = Keyboard::RShift;
      m_keyCodes[VK_LCONTROL] = Keyboard::LControl;
      m_keyCodes[VK_RCONTROL] = Keyboard::RControl;
      m_keyCodes[VK_LMENU] = Keyboard::LAlt;
      m_keyCodes[VK_RMENU] = Keyboard::RAlt;
      m_keyCodes[VK_BROWSER_BACK] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_FORWARD] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_REFRESH] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_STOP] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_SEARCH] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_FAVORITES] = Keyboard::Unknown;
      m_keyCodes[VK_BROWSER_HOME] = Keyboard::Unknown;
      m_keyCodes[VK_VOLUME_MUTE] = Keyboard::Unknown;
      m_keyCodes[VK_VOLUME_DOWN] = Keyboard::Unknown;
      m_keyCodes[VK_VOLUME_UP] = Keyboard::Unknown;
      m_keyCodes[VK_MEDIA_NEXT_TRACK] = Keyboard::Unknown;
      m_keyCodes[VK_MEDIA_PREV_TRACK] = Keyboard::Unknown;
      m_keyCodes[VK_MEDIA_STOP] = Keyboard::Unknown;
      m_keyCodes[VK_MEDIA_PLAY_PAUSE] = Keyboard::Unknown;
      m_keyCodes[VK_LAUNCH_MAIL] = Keyboard::Unknown;
      m_keyCodes[VK_LAUNCH_MEDIA_SELECT] = Keyboard::Unknown;
      m_keyCodes[VK_LAUNCH_APP1] = Keyboard::Unknown;
      m_keyCodes[VK_LAUNCH_APP2] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_1] = Keyboard::Semicolon;
      m_keyCodes[VK_OEM_PLUS] = Keyboard::Equals;
      m_keyCodes[VK_OEM_COMMA] = Keyboard::Comma;
      m_keyCodes[VK_OEM_MINUS] = Keyboard::Minus;
      m_keyCodes[VK_OEM_PERIOD] = Keyboard::Period;
      m_keyCodes[VK_OEM_2] = Keyboard::Slash;
      m_keyCodes[VK_OEM_3] = Keyboard::Tilde;
      m_keyCodes[VK_OEM_4] = Keyboard::OpenSquareBracket;
      m_keyCodes[VK_OEM_5] = Keyboard::Backslash;
      m_keyCodes[VK_OEM_6] = Keyboard::CloseSquareBracket;
      m_keyCodes[VK_OEM_7] = Keyboard::Apostrophe;
      m_keyCodes[VK_OEM_8] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_AX] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_102] = Keyboard::Unknown;
      m_keyCodes[VK_ICO_HELP] = Keyboard::Unknown;
      m_keyCodes[VK_ICO_00] = Keyboard::Unknown;
      m_keyCodes[VK_PROCESSKEY] = Keyboard::Unknown;
      m_keyCodes[VK_ICO_CLEAR] = Keyboard::Unknown;
      m_keyCodes[VK_PACKET] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_RESET] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_JUMP] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_PA1] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_PA2] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_PA3] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_WSCTRL] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_CUSEL] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_ATTN] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_FINISH] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_COPY] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_AUTO] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_ENLW] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_BACKTAB] = Keyboard::Unknown;
      m_keyCodes[VK_ATTN] = Keyboard::Unknown;
      m_keyCodes[VK_CRSEL] = Keyboard::Unknown;
      m_keyCodes[VK_EXSEL] = Keyboard::Unknown;
      m_keyCodes[VK_EREOF] = Keyboard::Unknown;
      m_keyCodes[VK_PLAY] = Keyboard::Unknown;
      m_keyCodes[VK_ZOOM] = Keyboard::Unknown;
      m_keyCodes[VK_NONAME] = Keyboard::Unknown;
      m_keyCodes[VK_PA1] = Keyboard::Unknown;
      m_keyCodes[VK_OEM_CLEAR] = Keyboard::Unknown;
   }

}
