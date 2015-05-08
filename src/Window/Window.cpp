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
#include "util/utility.h"
#include "util/geometry/vec2.h"
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
   {
   }

   Window::Window(const char* title)
      : m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
      m_xPos(CW_USEDEFAULT), m_yPos(CW_USEDEFAULT),
      m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
      m_exitCode(0), m_style(0), m_extendedStyle(0),
      m_minFileChangeDelay(200), m_fileChangeDelay(m_minFileChangeDelay),
      m_headIndex(1), m_tailIndex(0), m_eventQueueSize(1024),
      m_hwnd(nullptr), m_console(nullptr),
      m_fullscreen(false), m_showCursor(false), m_lockCursor(false), m_relativeMouse(false),
      m_isRunning(true),
      m_class(title), m_title(title)
   {
      m_events.resize(m_eventQueueSize);

      getProxy().setFullscreen(m_fullscreen);
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

   void Window::update()
   {
      if( !m_hwnd )
      {
         MessageBox(nullptr, "Window::HandleMessage(): The window has not been created yet.", "Runtime error", MB_OK);
         m_exitCode = WindowResult::WindowNotExistsError;
         m_isRunning = false;
         return;
      }
      static MSG msg;
      while( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
      {
         if( msg.message == WM_QUIT )
         {
            m_exitCode = msg.wParam;
            m_isRunning = false;
            return;
         }
         TranslateMessage(&msg);
         DispatchMessage(&msg);

         if( m_mouseHandler.handle(newEvent(), msg.message, msg.wParam, msg.lParam) )
         {
            writeEvent();
         }
         if( m_keyboardHandler.handle(newEvent(), msg.message, msg.wParam, msg.lParam) )
         {
            writeEvent();
         }
      }
      auto events = m_gamepadHandler.handle(Clock::getRealTimeMicros());
      for( auto event : events )
      {
         auto& we = newEvent();
         we = event;
         writeEvent();
      }
      processFileChanges();
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
      //here we translate all WM_* messages to the Core::WindowMessage
      bool eventMapped = true;
      LRESULT result = 0;
      const LRESULT notProcessed = -1;
      auto& we = newEvent();
      switch( msg )
      {
         case WM_CLOSE:
         {
            we.type = WindowEventType::WE_CLOSE;
            result = notProcessed;
         } break;

         case WM_SETCURSOR:
         {
            POINT cur;
            GetCursorPos(&cur);
            RECT rc;
            GetClientRect(m_hwnd, &rc);
            ScreenToClient(m_hwnd, &cur);
            result = notProcessed;
            if( !m_showCursor && cur.y >= rc.top && cur.y <= rc.bottom && cur.x >= rc.left && cur.x <= rc.right )
            {
               SetCursor(nullptr);
               result = TRUE;
            }
            eventMapped = false;
         } break;

         case WM_ACTIVATE:
         {
            if( LOWORD(wParam) == WA_INACTIVE )
            {
               we.type = WindowEventType::WE_LOSTFOCUS;
            }
            else
            {
               we.type = WindowEventType::WE_GAINFOCUS;
               getProxy().lockCursor(m_lockCursor);
               getProxy().showCursor(m_showCursor);
            }
            result = notProcessed;
         } break;

         default:
         {
            eventMapped = false;
            result = notProcessed;
         }
         break;
      }

      if( eventMapped )
      {
         writeEvent();
      }

      return result == notProcessed ? DefWindowProc(hwnd, msg, wParam, lParam) : result;
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
         CORE_INFO("WHOOPS, overwriting a previous event. This is a BAD THING! Maybe we "
                   "should increase the size of our buffer from ", m_eventQueueSize, "...");
      }

      m_headIndex = (m_headIndex + 1) % m_eventQueueSize;
   }

   void Window::processFileChanges()
   {
      std::string file;
      DWORD action;
      while( m_monitor.Pop(action, file) )
      {
         if( file.size() < FilenameStringSize && file.find(".") != file.npos )
         {
            auto& we = newEvent();
            we.type = WE_FILECHANGE;
            we.fileChange.action = toFileChangeType(action);
            strncpy(we.fileChange.filename, file.c_str(), FilenameStringSize);
            we.fileChange.filename[FilenameStringSize] = 0;
            writeEvent();
         }
         else
         {
            CORE_INFO("Filename too long (", file.size(), ") or not a file(", file, ")");
         }
      }
   }

   void calculateClientRect(uint32_t x, uint32_t y, uint32_t style, uint32_t styleEx, uint32_t& outXSize, uint32_t& outYSize)
   {
      RECT rc = {0, 0, x, y};
      AdjustWindowRectEx(&rc, styleEx, false, styleEx);
      outXSize = rc.right - rc.left;
      outYSize = rc.bottom - rc.top;
   }
}