//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/mouse_handler.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "input/mouse.h"
#include "util/communication_buffer.h"
/******* end headers *******/

namespace core
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

   void MouseHandler::handle(CommunicationBuffer* buffer, uint32_t msg, WPARAM wp, LPARAM lp)
   {
      WindowEvent we;
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
                  we.type = WindowEventType::WE_MOUSEMOVE;
                  we.mouse.position.x = x;
                  we.mouse.position.y = y;
                  we.mouse.move.relative = true;
                  SetCursorPos(m_centerX, m_centerY);
               }
            }
            else
            {
               we.type = WindowEventType::WE_MOUSEMOVE;
               we.mouse.position.x = GET_X_LPARAM(lp);
               we.mouse.position.y = GET_Y_LPARAM(lp);
               we.mouse.move.relative = false;
            }
         } break;
         
         case WM_LBUTTONDOWN:
         case WM_RBUTTONDOWN:
         case WM_MBUTTONDOWN:
         case WM_XBUTTONDOWN:
         {
            matched = true;
            we.type = WindowEventType::WE_MOUSEBUTTON;
            we.mouse.position.x = GET_X_LPARAM(lp);
            we.mouse.position.y = GET_Y_LPARAM(lp);
            we.mouse.button.id = toMouseKey(msg, wp);
            we.mouse.button.isDown = true;
         } break;

         case WM_LBUTTONUP:
         case WM_RBUTTONUP:
         case WM_MBUTTONUP:
         case WM_XBUTTONUP:
         {
            matched = true;
            we.type = WindowEventType::WE_MOUSEBUTTON;
            we.mouse.position.x = GET_X_LPARAM(lp);
            we.mouse.position.y = GET_Y_LPARAM(lp);
            we.mouse.button.id = toMouseKey(msg, wp);
            we.mouse.button.isDown = false;
         } break;

         case WM_MOUSEWHEEL:
         {
            matched = true;
            we.type = WindowEventType::WE_MOUSEWHEEL;
            we.mouse.position.x = GET_X_LPARAM(lp);
            we.mouse.position.y = GET_Y_LPARAM(lp);
            we.mouse.wheel.scroll = GET_WHEEL_DELTA_WPARAM(wp);
         } break;
      }
      if( matched )
      {
         buffer->writeEvent(we);
      }
   }
}
