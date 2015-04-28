//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <window/window.h>
/******* c++ headers *******/
#include <algorithm>
#include <cstdlib>
#include <iostream>
/******* extra headers *******/
#include <util/clock.h>
#include <util/time_unit_conversions.h>
#include <util/utility.h>
#include <util/vec2.h>
#include <window/window_class.h>
/******* end headers *******/

namespace Core
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

   Window::Window(const std::string& title)
      : Window(title.c_str())
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

      setFullscreen(m_fullscreen);
   }

   Window::~Window()
   {
      m_monitor.Terminate();
   }

   bool Window::create()
   {
      uint32_t x, y;
      calculateClientRect(x, y);
      HWND hwndParent = nullptr;
      HMENU hMenu = nullptr;
      HINSTANCE hInstance = nullptr;

      m_hwnd = CreateWindowEx(
         m_extendedStyle,
         m_class.c_str(),
         m_title.c_str(),
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

   void Window::close()
   {
      m_lockCursor = false;
      InvalidateRect(m_hwnd, nullptr, true);
#ifndef _DEBUG
      PostMessage(m_hwnd, WM_CLOSE, 0, 0);
      if( m_console )
      {
         closeConsole();
      }
#else
      SetForegroundWindow(m_console);
#endif
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
   
   const std::string& Window::getClass() const
   {
      return m_class;
   }

   const std::string& Window::getTitle() const
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
   
   bool Window::isRunning() const
   {
      return m_isRunning;
   }

   void Window::openConsole(int32_t xPos, int32_t yPos)
   {
      if( !AllocConsole() )
         return;

      freopen("CONIN$", "r", stdin);
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);

      char name[]
      {
         "CoreDebugConsole"
      };
      SetConsoleTitle(name);
      Sleep(40);
      m_console = FindWindow(nullptr, name);
      RECT consoleSize;
      GetWindowRect(m_console, &consoleSize);
      auto x = consoleSize.right - consoleSize.left;
      auto y = consoleSize.bottom - consoleSize.top;
      SetWindowPos(m_console, 0, xPos, yPos, x, y, 0); //SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW
      SetForegroundWindow(m_hwnd);
   }

   void Window::closeConsole()
   {
      FreeConsole();
      m_console = nullptr;
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
               lockCursor(m_lockCursor);
               showCursor(m_showCursor);
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
}