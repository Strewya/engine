//headers should be ordered alphabetically!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <window/window.h>
/******* c++ headers *******/
/******* extra headers *******/
#include <util/utility.h>
/******* end headers *******/

namespace Core
{
   void Window::showMessagebox(const char* title, const char* text)
   {
      MessageBox(m_hwnd, text, title, MB_OK);
   }

   void Window::resize(uint32_t x, uint32_t y)
   {
      m_xSize = (x == CW_USEDEFAULT || x == 0) ? GetSystemMetrics(SM_CXSCREEN) : x;
      m_ySize = (y == CW_USEDEFAULT || y == 0) ? GetSystemMetrics(SM_CYSCREEN) : y;
      calculateClientRect(x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void Window::move(int32_t xp, int32_t yp)
   {
      m_xPos = xp == CW_USEDEFAULT ? 0 : xp;
      m_yPos = yp == CW_USEDEFAULT ? 0 : yp;
      uint32_t x, y;
      calculateClientRect(x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   void Window::showCursor(bool isShown)
   {
      m_showCursor = isShown;
   }

   void Window::lockCursor(bool isLocked)
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

   void Window::makeMouseRelative(bool isRelative)
   {
      m_mouseHandler.setRelativeMouseMove(isRelative, m_xSize / 2, m_ySize / 2);
   }

   void Window::setFullscreen(bool fs)
   {
      m_fullscreen = fs;
      if( fs )
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
      uint32_t x, y;
      calculateClientRect(x, y);
      SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
   }

   int32_t Window::getPositionX() const
   {
      return m_xPos;
   }
   
   int32_t Window::getPositionY() const
   {
      return m_yPos;
   }
   
   uint32_t Window::getSizeX() const
   {
      return m_xSize;
   }
   
   uint32_t Window::getSizeY() const
   {
      return m_ySize;
   }

   bool Window::isCursorShown() const
   {
      return m_showCursor;
   }
   
   bool Window::isFullscreen() const
   {
      return m_fullscreen;
   }

   void Window::monitorDirectoryForChanges(const char* directory)
   {
      char lpName[128] = {0};
      GetCurrentDirectory(128, lpName);
      std::string dir(lpName);
      auto pos = dir.find_last_of('\\');
      dir.assign(dir.substr(0, pos + 1)).append(directory).shrink_to_fit();

      m_monitor.AddDirectory(dir.c_str(), true, m_trackedChanges);
   }

   void Window::setFileChangeDelay(uint32_t delay)
   {
      m_fileChangeDelay = (delay > m_minFileChangeDelay ? delay : m_minFileChangeDelay);
   }

   std::vector<WindowEvent> Window::collectEvents(uint64_t time)
   {
      std::vector<WindowEvent> events;
      
      auto readIndex = (m_tailIndex + 1) % m_eventQueueSize;
      while( readIndex != m_headIndex && m_events[readIndex].timestamp <= time )
      {
         events.push_back(m_events[readIndex]);
         m_tailIndex = readIndex;
         readIndex = (m_tailIndex + 1) % m_eventQueueSize;
      }
      return events;
   }
}
