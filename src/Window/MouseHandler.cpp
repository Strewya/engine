//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Window/MouseHandler.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Input/Mouse.h>
/******* end headers *******/

namespace Core
{
   MouseHandler::MouseHandler()
      : m_centerX(0), m_centerY(0), m_relativeMouseMove(false)
   {
   }

   void MouseHandler::setRelativeMouseMove(bool isRelative, uint32_t centerX, uint32_t centerY)
   {
      m_relativeMouseMove = isRelative;
      m_centerX = centerX;
      m_centerY = centerY;
   }

   Mouse::Key toMouseKey(uint32_t code, WPARAM wParam)
   {
      Mouse::Key key = Mouse::KeyCount;
      switch( code )
      {
         case WM_LBUTTONDBLCLK:
         case WM_LBUTTONDOWN:
         case WM_LBUTTONUP:
         {
            key = Mouse::LeftButton;
         } break;

         case WM_RBUTTONDBLCLK:
         case WM_RBUTTONDOWN:
         case WM_RBUTTONUP:
         {
            key = Mouse::RightButton;
         } break;

         case WM_MBUTTONDBLCLK:
         case WM_MBUTTONDOWN:
         case WM_MBUTTONUP:
         {
            key = Mouse::MiddleButton;
         } break;

         case WM_XBUTTONDBLCLK:
         case WM_XBUTTONDOWN:
         case WM_XBUTTONUP:
         {
            key = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
         } break;
      }
      return key;
   }

   bool MouseHandler::handle(WindowEvent& we, uint32_t msg, WPARAM wp, LPARAM lp)
   {
      bool matched = false;
      switch( msg )
      {
         case WM_MOUSEMOVE:
         {
            matched = true;
            if( m_relativeMouseMove )
            {
               auto x = GET_X_LPARAM(lp) - m_centerX;
               auto y = GET_Y_LPARAM(lp) - m_centerY;
               if( x != 0 || y != 0 )
               {
                  we.m_type = WindowEventType::WE_MOUSEMOVE;
                  we.m_mouseMove.m_x = x;
                  we.m_mouseMove.m_y = y;
                  we.m_mouseMove.m_isRelative = true;
                  SetCursorPos(m_centerX, m_centerY);
               }
            }
            else
            {
               we.m_type = WindowEventType::WE_MOUSEMOVE;
               we.m_mouseMove.m_x = GET_X_LPARAM(lp);
               we.m_mouseMove.m_y = GET_Y_LPARAM(lp);
               we.m_mouseMove.m_isRelative = false;
            }
         } break;
         
         case WM_LBUTTONDOWN:
         case WM_RBUTTONDOWN:
         case WM_MBUTTONDOWN:
         case WM_XBUTTONDOWN:
         {
            matched = true;
            we.m_type = WindowEventType::WE_MOUSEBUTTON;
            we.m_mouseButton.m_clicks = 1;
            we.m_mouseButton.m_isDown = true;
            we.m_mouseButton.m_x = GET_X_LPARAM(lp);
            we.m_mouseButton.m_y = GET_Y_LPARAM(lp);
            we.m_mouseButton.m_button = toMouseKey(msg, wp);
         } break;

         case WM_LBUTTONUP:
         case WM_RBUTTONUP:
         case WM_MBUTTONUP:
         case WM_XBUTTONUP:
         {
            matched = true;
            we.m_type = WindowEventType::WE_MOUSEBUTTON;
            we.m_mouseButton.m_clicks = 0;
            we.m_mouseButton.m_isDown = false;
            we.m_mouseButton.m_x = GET_X_LPARAM(lp);
            we.m_mouseButton.m_y = GET_Y_LPARAM(lp);
            we.m_mouseButton.m_button = toMouseKey(msg, wp);
         } break;

         case WM_MOUSEWHEEL:
         {
            matched = true;
            we.m_type = WindowEventType::WE_MOUSEWHEEL;
            we.m_mouseWheel.m_scroll = GET_WHEEL_DELTA_WPARAM(wp);
            we.m_mouseButton.m_x = GET_X_LPARAM(lp);
            we.m_mouseButton.m_y = GET_Y_LPARAM(lp);
         } break;
      }
      return matched;
   }
}
