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
#include "utility/time/clock.h"
#include "utility/time/time_unit_conversions.h"
#include "utility/communication_buffer.h"
#include "utility/utility.h"
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

   Window::Window(const char* title) :
      m_class(title), m_title(title),
      m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
      m_xPos(0), m_yPos(0),
      m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
      m_exitCode(0), m_style(0), m_extendedStyle(0),
      m_minFileChangeDelay(200), m_fileChangeDelay(m_minFileChangeDelay),
      m_hwnd(nullptr),
      m_fullscreen(false), m_showCursor(false), m_lockCursor(false), m_relativeMouse(false),
      m_isRunning(true),
      m_readAsyncIndex(0), m_writeAsyncIndex(1)
   {}

   Window::~Window()
   {
      m_monitor.Terminate();
      setLockCursor(false);
      setShowCursor(true);
      setRelativeCursor(false);
   }

   bool Window::create()
   {
      u32 x, y;
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

   bool Window::processWin32Messages(CommunicationBuffer* toGame)
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
            WinMsg we{};
            we.type = WinMsgType::Close;
            toGame->writeEvent(we);
         }
         //write the mouse event to the input queue that goes to the game
         m_mouseHandler.handle(toGame, msg.message, msg.wParam, msg.lParam);
         m_keyboardHandler.handle(toGame, msg.message, msg.wParam, msg.lParam);
      }
      m_gamepadHandler.handle(toGame, Clock::getRealTimeMicros());
      processFileChanges(toGame);
      while( (m_readAsyncIndex + 1) < m_writeAsyncIndex )
      {
         toGame->writeEvent(m_asyncMessages[(m_readAsyncIndex + 1) % AsyncMsgCount]);
         ++m_readAsyncIndex;
      }
      return true;
   }

   void Window::processCommands(CommunicationBuffer* fromGame, CommunicationBuffer* toGame)
   {
      core::WinMsg cmd;
      while( fromGame->peek(cmd) )
      {
         switch( cmd.type )
         {
            case core::WinMsgType::LockCursor:
            {
               setLockCursor(cmd.lockCursor);
            } break;
            case core::WinMsgType::ShowCursor:
            {
               setShowCursor(cmd.showCursor);
            } break;
            case core::WinMsgType::RelativeCursor:
            {
               setRelativeCursor(cmd.relativeCursor);
            } break;
            case core::WinMsgType::Fullscreen:
            {
               setFullscreen(cmd.fullscreen);
            } break;
            case core::WinMsgType::Size:
            {
               resize(cmd.screen.x, cmd.screen.y);
               WinMsg msg{};
               msg.type = core::WinMsgType::Size;
               msg.screen.x = getSizeX();
               msg.screen.y = getSizeY();
               toGame->writeEvent(msg);
            } break;
            case core::WinMsgType::Position:
            {
               move(cmd.screen.x, cmd.screen.y);
            } break;
            case core::WinMsgType::FileChange:
            {
               monitorDirectoryForChanges(cmd.fileChange.name);
            } break;
            case core::WinMsgType::Close:
            {
               close();
            } break;
         }
      }
   }

   void Window::setStyle(u32 style)
   {
      m_style = style;
   }

   void Window::setExtendedStyle(u32 style)
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

   u32 Window::getStyle() const
   {
      return m_style;
   }

   u32 Window::getExtendedStyle() const
   {
      return m_extendedStyle;
   }

   i32 Window::getExitCode() const
   {
      return m_exitCode;
   }

   void Window::close() const
   {
      InvalidateRect(m_hwnd, nullptr, true);
      PostMessage(m_hwnd, WM_CLOSE, 0, 0);
   }

   void Window::showMessagebox(const char* title, const char* text) const
   {
      MessageBox(nullptr, text, title, MB_OK);
   }

   void Window::resize(u32 x, u32 y)
   {
      m_xSize = (x == CW_USEDEFAULT || x == 0) ? GetSystemMetrics(SM_CXSCREEN) : x;
      m_ySize = (y == CW_USEDEFAULT || y == 0) ? GetSystemMetrics(SM_CYSCREEN) : y;
      calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void Window::move(i32 xp, i32 yp)
   {
      m_xPos = xp == CW_USEDEFAULT ? 0 : xp;
      m_yPos = yp == CW_USEDEFAULT ? 0 : yp;
      u32 x, y;
      calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void Window::setShowCursor(bool isShown)
   {
      m_showCursor = isShown;
   }

   void Window::setLockCursor(bool isLocked)
   {
      m_lockCursor = isLocked;
      if( isLocked )
      {
         RECT screen{0, 0, m_xSize, m_ySize};
         ClipCursor(&screen);
      }
      else
      {
         ClipCursor(nullptr);
      }
   }

   void Window::setRelativeCursor(bool isRelative)
   {
      m_mouseHandler.setRelativeMouseMove(isRelative, m_xSize / 2, m_ySize / 2);
   }

   void Window::setFullscreen(bool isFullscreen)
   {
      m_fullscreen = isFullscreen;
      if( isFullscreen )
      {
         setStyle(WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
         setExtendedStyle(WS_EX_APPWINDOW);
      }
      else
      {
         setStyle(WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);
         setExtendedStyle(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
      }

      SetWindowLong(m_hwnd, GWL_STYLE, m_style);
      SetWindowLong(m_hwnd, GWL_EXSTYLE, m_extendedStyle);
      u32 x, y;
      calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   i32 Window::getPositionX() const
   {
      return m_xPos;
   }

   i32 Window::getPositionY() const
   {
      return m_yPos;
   }

   u32 Window::getSizeX() const
   {
      return m_xSize;
   }

   u32 Window::getSizeY() const
   {
      return m_ySize;
   }

   void Window::monitorDirectoryForChanges(const char* directory)
   {
      char lpName[128] = {0};
      GetCurrentDirectory(128, lpName);
      std::string dir(lpName);
      dir.append("\\").append(directory).shrink_to_fit();

      m_monitor.AddDirectory(dir.c_str(), true, m_trackedChanges);
   }

   void Window::setFileChangeDelay(u32 delay)
   {
      m_fileChangeDelay = (delay > m_minFileChangeDelay ? delay : m_minFileChangeDelay);
   }

   LRESULT CALLBACK Window::messageRouter(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam)
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

   LRESULT WINAPI Window::windowProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam)
   {
      //here we translate window related WM_* messages to Core::WindowEvent
      LRESULT result = 0;
      switch( msg )
      {
         case WM_ACTIVATE:
         {
            if( LOWORD(wParam) == WA_INACTIVE )
            {
               m_asyncMessages[m_writeAsyncIndex%AsyncMsgCount].type = WinMsgType::LostFocus;
            }
            else
            {
               m_asyncMessages[m_writeAsyncIndex%AsyncMsgCount].type = WinMsgType::GainFocus;
            }
            ++m_writeAsyncIndex;
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

   core_internal FileChangeType toFileChangeType(DWORD action)
   {
      FileChangeType returnValue = core::FileChangeType::BadData;
      switch( action )
      {
         case FILE_ACTION_ADDED:
         {
            returnValue = core::FileChangeType::Added;
         }
         break;
         case FILE_ACTION_MODIFIED:
         {
            returnValue = core::FileChangeType::Modified;
         }
         break;
         case FILE_ACTION_REMOVED:
         {
            returnValue = core::FileChangeType::Removed;
         }
         break;
         case FILE_ACTION_RENAMED_OLD_NAME:
         {
            returnValue = core::FileChangeType::RenamedFrom;
         }
         break;
         case FILE_ACTION_RENAMED_NEW_NAME:
         {
            returnValue = core::FileChangeType::RenamedTo;
         }
         break;
         default:
         {
         } break;
      }
      return returnValue;
   }

   void Window::processFileChanges(CommunicationBuffer* buffer)
   {
      std::string file;
      DWORD action;
      while( m_monitor.Pop(action, file) )
      {
         if( file.size() < FileChangeEvent::NameStringSize && file.find(".") != file.npos )
         {
            WinMsg we{};
            we.type = WinMsgType::FileChange;
            we.fileChange.action = toFileChangeType(action);
            strncpy(we.fileChange.name, file.c_str(), FileChangeEvent::NameStringSize);
            we.fileChange.name[FileChangeEvent::NameStringSize] = 0;
            buffer->writeEvent(we);
         }
         else
         {
            CORE_LOG_DEBUG("Filename too long (", file.size(), ") or not a file(", file, ")");
         }
      }
   }

   void calculateClientRect(u32 x, u32 y, u32 style, u32 styleEx, u32& outXSize, u32& outYSize)
   {
      RECT rc = {0, 0, x, y};
      AdjustWindowRectEx(&rc, style, false, styleEx);
      outXSize = rc.right - rc.left;
      outYSize = rc.bottom - rc.top;
   }
}