//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/window_proxy.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "window/window.h"
/******* end headers *******/

namespace core
{
   WindowProxy::WindowProxy(Window* window)
      : m_window(window)
   {
   }

   bool WindowProxy::isValid() const
   {
      return m_window != nullptr;
   }

   void WindowProxy::close() const
   {
      m_window->m_lockCursor = false;
      InvalidateRect(m_window->m_hwnd, nullptr, true);
#ifndef _DEBUG
      PostMessage(m_window->m_hwnd, WM_CLOSE, 0, 0);
      if( m_window->m_console )
      {
         closeConsole();
      }
#else
      SetForegroundWindow(m_window->m_console);
#endif
   }

   void WindowProxy::showMessagebox(const char* title, const char* text) const
   {
      MessageBox(m_window->m_hwnd, text, title, MB_OK);
   }

   void WindowProxy::resize(uint32_t x, uint32_t y) const
   {
      m_window->m_xSize = (x == CW_USEDEFAULT || x == 0) ? GetSystemMetrics(SM_CXSCREEN) : x;
      m_window->m_ySize = (y == CW_USEDEFAULT || y == 0) ? GetSystemMetrics(SM_CYSCREEN) : y;
      calculateClientRect(m_window->m_xSize, m_window->m_ySize, m_window->m_style, m_window->m_extendedStyle, x, y);
      SetWindowPos(m_window->m_hwnd, 0, m_window->m_xPos, m_window->m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void WindowProxy::move(int32_t xp, int32_t yp) const
   {
      m_window->m_xPos = xp == CW_USEDEFAULT ? 0 : xp;
      m_window->m_yPos = yp == CW_USEDEFAULT ? 0 : yp;
      uint32_t x, y;
      calculateClientRect(m_window->m_xSize, m_window->m_ySize, m_window->m_style, m_window->m_extendedStyle, x, y);
      SetWindowPos(m_window->m_hwnd, 0, m_window->m_xPos, m_window->m_yPos, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void WindowProxy::showCursor(bool isShown) const
   {
      m_window->m_showCursor = isShown;
   }

   void WindowProxy::lockCursor(bool isLocked) const
   {
      m_window->m_lockCursor = isLocked;
      if( isLocked )
      {
         RECT screen{0, 0, m_window->m_xSize, m_window->m_ySize};
         ClipCursor(&screen);
      }
      else
      {
         ClipCursor(nullptr);
      }
   }

   void WindowProxy::makeMouseRelative(bool isRelative) const
   {
      m_window->m_mouseHandler.setRelativeMouseMove(isRelative, m_window->m_xSize / 2, m_window->m_ySize / 2);
   }

   void WindowProxy::setFullscreen(bool isFullscreen) const
   {
      m_window->m_fullscreen = isFullscreen;
      if( isFullscreen )
      {
         m_window->setStyle(WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
         m_window->setExtendedStyle(WS_EX_APPWINDOW);
      }
      else
      {
         m_window->setStyle(WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);
         m_window->setExtendedStyle(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
      }

      SetWindowLong(m_window->m_hwnd, GWL_STYLE, m_window->m_style);
      SetWindowLong(m_window->m_hwnd, GWL_EXSTYLE, m_window->m_extendedStyle);
      uint32_t x, y;
      calculateClientRect(m_window->m_xSize, m_window->m_ySize, m_window->m_style, m_window->m_extendedStyle, x, y);
      SetWindowPos(m_window->m_hwnd, 0, m_window->m_xPos, m_window->m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   int32_t WindowProxy::getPositionX() const
   {
      return m_window->m_xPos;
   }

   int32_t WindowProxy::getPositionY() const
   {
      return m_window->m_yPos;
   }

   uint32_t WindowProxy::getSizeX() const
   {
      return m_window->m_xSize;
   }

   uint32_t WindowProxy::getSizeY() const
   {
      return m_window->m_ySize;
   }

   bool WindowProxy::isCursorShown() const
   {
      return m_window->m_showCursor;
   }

   bool WindowProxy::isFullscreen() const
   {
      return m_window->m_fullscreen;
   }

   bool WindowProxy::isRunning() const
   {
      return m_window->m_isRunning;
   }

   void WindowProxy::openConsole(int32_t xPos, int32_t yPos) const
   {
      if( m_window->m_console || !AllocConsole() )
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
      m_window->m_console = FindWindow(nullptr, name);
      RECT consoleSize;
      GetWindowRect(m_window->m_console, &consoleSize);
      auto x = consoleSize.right - consoleSize.left;
      auto y = consoleSize.bottom - consoleSize.top;
      SetWindowPos(m_window->m_console, 0, xPos, yPos, x, y, 0); //SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW
      SetForegroundWindow(m_window->m_hwnd);
   }

   void WindowProxy::closeConsole() const
   {
      FreeConsole();
      m_window->m_console = nullptr;
   }

   void WindowProxy::monitorDirectoryForChanges(const char* directory) const
   {
      char lpName[128] = {0};
      GetCurrentDirectory(128, lpName);
      std::string dir(lpName);
      auto pos = dir.find_last_of('\\');
      dir.assign(dir.substr(0, pos + 1)).append(directory).shrink_to_fit();

      m_window->m_monitor.AddDirectory(dir.c_str(), true, m_window->m_trackedChanges);
   }

   void WindowProxy::setFileChangeDelay(uint32_t delay) const
   {
      m_window->m_fileChangeDelay = (delay > m_window->m_minFileChangeDelay ? delay : m_window->m_minFileChangeDelay);
   }

   std::vector<WindowEvent> WindowProxy::collectEvents(uint64_t time) const
   {
      std::vector<WindowEvent> events;

      auto readIndex = (m_window->m_tailIndex + 1) % m_window->m_eventQueueSize;
      while( readIndex != m_window->m_headIndex && m_window->m_events[readIndex].timestamp <= time )
      {
         events.push_back(m_window->m_events[readIndex]);
         m_window->m_tailIndex = readIndex;
         readIndex = (m_window->m_tailIndex + 1) % m_window->m_eventQueueSize;
      }
      return events;
   }

   Window* WindowProxy::getRawWindow() const
   {
      return m_window;
   }
}