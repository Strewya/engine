//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/window.h"
/******* c++ headers *******/
#include <algorithm>
#include <cstdlib>
#include <iostream>
/******* extra headers *******/
#include "util/time/clock.h"
#include "util/time/time_unit_conversions.h"
#include "util/communication_buffer.h"
#include "util/utility.h"
#include "window/window_class.h"
/******* end headers *******/

namespace core
{
   WindowResult initializeWindow(Window& window)
   {
      WindowClass wndClass(window.getClass());

      if( wndClass.registerClass() == 0 )
      {
         MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
         return WindowResult::WindowClassRegistrationError;
      }

      if( !window.create() )
      {
         MessageBox(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
         return WindowResult::WindowCreationError;
      }

      window.show();
      return WindowResult::OK;
   }


   Window::Window()
      : Window("Window")
   {}

   Window::Window(const char* title)
      : m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
      m_xPos(CW_USEDEFAULT), m_yPos(CW_USEDEFAULT),
      m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
      m_exitCode(0), m_style(0), m_extendedStyle(0),
      m_minFileChangeDelay(200), m_fileChangeDelay(m_minFileChangeDelay),
      m_headIndex(1), m_tailIndex(0), m_eventQueueSize(1024),
      m_hwnd(nullptr),
      m_fullscreen(false), m_showCursor(false), m_lockCursor(false), m_relativeMouse(false),
      m_isRunning(true),
      m_class(title), m_title(title)
   {
      m_events.resize(m_eventQueueSize);
   }

   Window::~Window()
   {
      m_monitor.Terminate();
   }

   WindowProxy Window::getProxy()
   {
      return WindowProxy{this};
   }

   bool Window::create()
   {
      uint32_t x, y;
      calculateClientRect(m_xPos, m_yPos, m_style, m_extendedStyle, x, y);
      HWND hwndParent = nullptr;
      HMENU hMenu = nullptr;
      HINSTANCE hInstance = nullptr;

      m_hwnd = CreateWindowEx(
         m_extendedStyle,
         m_class,
         m_title,
         m_style,
         m_xPos,
         m_yPos,
         x,
         y,
         hwndParent,
         hMenu,
         hInstance,
         this
         );

      return m_hwnd != nullptr;
   }

   void Window::show()
   {
      ShowWindow(m_hwnd, SW_SHOW);
      UpdateWindow(m_hwnd);
   }

   bool Window::processWin32Messages(CommunicationBuffer* communication)
   {
      MSG msg;
      while( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
      {
         if( msg.message == WM_QUIT )
         {
            m_exitCode = msg.wParam;
            return false;
         }
         TranslateMessage(&msg);
         DispatchMessage(&msg);

         if( msg.message == WM_CLOSE )
         {
            WindowEvent we{};
            we.type = WindowEventType::WE_CLOSE;
            communication->writeEvent(we);
         }
         else if( msg.message == WM_ACTIVATE )
         {
            PostQuitMessage(0);
            WindowEvent we{};
            if( LOWORD(msg.wParam) == WA_INACTIVE )
            {
               we.type = WindowEventType::WE_LOSTFOCUS;
            }
            else
            {
               we.type = WindowEventType::WE_GAINFOCUS;
            }
            communication->writeEvent(we);
         }
         //write the mouse event to the input queue that goes to the game
         m_mouseHandler.handle(communication, msg.message, msg.wParam, msg.lParam);
         m_keyboardHandler.handle(communication, msg.message, msg.wParam, msg.lParam);
      }
      m_gamepadHandler.handle(communication, Clock::getRealTimeMicros());
      processFileChanges(communication);
      return true;
   }

   void Window::setStyle(uint32_t style)
   {
      m_style = style;
   }

   void Window::setExtendedStyle(uint32_t style)
   {
      m_extendedStyle = style;
   }

   const char* Window::getClass() const
   {
      return m_class;
   }

   const char* Window::getTitle() const
   {
      return m_title;
   }

   HWND Window::getWindowHandle() const
   {
      return m_hwnd;
   }

   uint32_t Window::getStyle() const
   {
      return m_style;
   }

   uint32_t Window::getExtendedStyle() const
   {
      return m_extendedStyle;
   }

   int32_t Window::getExitCode() const
   {
      return m_exitCode;
   }

   LRESULT CALLBACK Window::messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
   {
      Window* window = nullptr;

      if( msg == WM_NCCREATE )
      {
         window = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
         ::SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<long>(window));
         return ::DefWindowProc(hwnd, msg, wParam, lParam);
      }

      window = reinterpret_cast<Window*>(::GetWindowLong(hwnd, GWL_USERDATA));

      if( msg == WM_DESTROY )
      {
         PostQuitMessage(0);
         return 0;
      }

      if( window != nullptr )
      {
         return window->windowProc(hwnd, msg, wParam, lParam);
      }
      return ::DefWindowProc(hwnd, msg, wParam, lParam);
   }

   LRESULT WINAPI Window::windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
   {
      //here we translate window related WM_* messages to Core::WindowEvent
      LRESULT result = 0;
      switch( msg )
      {
         case WM_CLOSE:
         {
         } break;

         case WM_SETCURSOR:
         {
            POINT cur;
            GetCursorPos(&cur);
            RECT rc;
            GetClientRect(m_hwnd, &rc);
            ScreenToClient(m_hwnd, &cur);
            if( !m_showCursor && cur.y >= rc.top && cur.y <= rc.bottom && cur.x >= rc.left && cur.x <= rc.right )
            {
               SetCursor(nullptr);
               result = TRUE;
            }
            else
            {
               result = DefWindowProc(hwnd, msg, wParam, lParam);
            }
         } break;

         default:
         {
            result = DefWindowProc(hwnd, msg, wParam, lParam);
         }
         break;
      }

      return result;
   }

   static FileChangeType toFileChangeType(DWORD action)
   {
      FileChangeType returnValue = core::FILE_BADDATA;
      switch( action )
      {
         case FILE_ACTION_ADDED:
         {
            returnValue = core::FILE_ADDED;
         }
         break;

         case FILE_ACTION_MODIFIED:
         {
            returnValue = core::FILE_MODIFIED;
         }
         break;

         case FILE_ACTION_REMOVED:
         {
            returnValue = core::FILE_REMOVED;
         }
         break;

         case FILE_ACTION_RENAMED_OLD_NAME:
         {
            returnValue = core::FILE_RENAMED_FROM;
         }
         break;

         case FILE_ACTION_RENAMED_NEW_NAME:
         {
            returnValue = core::FILE_RENAMED_TO;
         }
         break;

         default:
            break;
      }
      return returnValue;
   }

   WindowEvent& Window::newEvent()
   {
      WindowEvent& we = m_events[m_headIndex];
      ZeroMemory(&we, sizeof(WindowEvent));
      we.timestamp = Clock::getRealTimeMicros();
      return we;
   }

   void Window::writeEvent()
   {
      if( m_headIndex == m_tailIndex )
      {
         CORE_LOG("WHOOPS, overwriting a previous input event. Maybe we "
                  "should increase the size of the buffer (currently ", m_eventQueueSize, ")...");
      }

      m_headIndex = (m_headIndex + 1) % m_eventQueueSize;
   }

   void Window::processFileChanges(CommunicationBuffer* buffer)
   {
      std::string file;
      DWORD action;
      while( m_monitor.Pop(action, file) )
      {
         if( file.size() < FilenameStringSize && file.find(".") != file.npos )
         {
            WindowEvent we{};
            we.type = WE_FILECHANGE;
            we.fileChange.action = toFileChangeType(action);
            strncpy(we.fileChange.filename, file.c_str(), FilenameStringSize);
            we.fileChange.filename[FilenameStringSize] = 0;
            buffer->writeEvent(we);
         }
         else
         {
            CORE_LOG_DEBUG("Filename too long (", file.size(), ") or not a file(", file, ")");
         }
      }
   }

   void calculateClientRect(uint32_t x, uint32_t y, uint32_t style, uint32_t styleEx, uint32_t& outXSize, uint32_t& outYSize)
   {
      RECT rc = {0, 0, x, y};
      AdjustWindowRectEx(&rc, style, false, styleEx);
      outXSize = rc.right - rc.left;
      outYSize = rc.bottom - rc.top;
   }
}