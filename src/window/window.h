#pragma once
/********************************************
*  contents:   Main window class
*  usage:
********************************************/
/******* c++ headers *******/
#include <atomic>
#include <string>
#include <vector>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
#include "util/types.h"
#include "window/gamepad_handler.h"
#include "window/keyboard_handler.h"
#include "window/mouse_handler.h"
#include "window/read_directory_changes.h"
#include "window/window_event.h"
#include "window/window_proxy.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;
   struct Window;

   enum WindowResult
   {
      OK = 0,
      WindowClassRegistrationError,
      WindowCreationError,
      InsufficientMemory,
   };

   WindowResult initializeWindow(Window& window);

   struct Window
   {
   public:
      WindowProxy getProxy();

      core_class_scope LRESULT CALLBACK messageRouter(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);

      Window();
      Window(const char* title);
      ~Window();

      //platform
      LRESULT CALLBACK windowProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);
      //platform
      bool create();
      //platform
      void show();
      //platform
      bool processWin32Messages(CommunicationBuffer* communication);

      //general if argument is enumerated, platform otherwise
      void setExtendedStyle(u32 style);
      //general if argument is enumerated, platform otherwise
      void setStyle(u32 style);
      //platform
      HWND getWindowHandle() const;
      //general if result is enumerated, platform otherwise
      u32 getStyle() const;
      //general if result is enumerated, platform otherwise
      u32 getExtendedStyle() const;
      //platform
      const char* getClass() const;
      //platform
      const char* getTitle() const;
      //platform
      i32 getExitCode() const;

   private:
      friend struct WindowProxy;
      //platform
      void processFileChanges(CommunicationBuffer* buffer);

      const char* m_class;
      const char* m_title;

      DWORD m_trackedChanges;
      i32 m_xPos;
      i32 m_yPos;
      i32 m_xSize;
      i32 m_ySize;
      u32 m_exitCode;
      u32 m_style;
      u32 m_extendedStyle;
      u32 m_minFileChangeDelay;
      u32 m_fileChangeDelay;
      const u32 m_eventQueueSize;

      HWND m_hwnd;

      bool m_fullscreen;
      bool m_showCursor;
      bool m_lockCursor;
      bool m_relativeMouse;
      bool m_isRunning;

      GamepadHandler m_gamepadHandler;
      MouseHandler m_mouseHandler;
      KeyboardHandler m_keyboardHandler;

      std::atomic<u32> m_headIndex;
      std::atomic<u32> m_tailIndex;
      std::vector<WindowEvent> m_events;

      CReadDirectoryChanges m_monitor;
   };

   void calculateClientRect(u32 x, u32 y, u32 style, u32 styleEx, u32& outXSize, u32& outYSize);
}
