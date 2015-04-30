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
#include "window/gamepad_handler.h"
#include "window/keyboard_handler.h"
#include "window/mouse_handler.h"
#include "window/read_directory_changes.h"
#include "window/window_event.h"
#include "window/window_proxy.h"
/******* end header inclusion *******/

namespace Core
{
   class Window;

   enum WindowResult
   {
      OK = 0,
      WindowClassRegistrationError,
      WindowCreationError,
      WindowNotExistsError,
      WindowClosing
   };

   WindowResult initializeWindow(Window& window);

   class Window
   {
   public:
      WindowProxy getProxy();

      static LRESULT CALLBACK messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

      Window();
      Window(const char* title);
      ~Window();

      //platform
      LRESULT CALLBACK windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
      //platform
      bool create();
      //platform
      void show();
      //platform
      void update();

      //general if argument is enumerated, platform otherwise
      void setExtendedStyle(uint32_t style);
      //general if argument is enumerated, platform otherwise
      void setStyle(uint32_t style);
      //platform
      HWND getWindowHandle() const;
      //general if result is enumerated, platform otherwise
      uint32_t getStyle() const;
      //general if result is enumerated, platform otherwise
      uint32_t getExtendedStyle() const;
      //platform
      const char* getClass() const;
      //platform
      const char* getTitle() const;
      //platform
      int32_t getExitCode() const;

   private:
      friend class WindowProxy;
      //platform
      WindowEvent& newEvent();
      //platform
      void writeEvent();
      //platform
      void newFileChange(uint64_t timestamp, DWORD action, const std::string& file);
      //platform
      void processFileChanges();

      const char* m_class;
      const char* m_title;

      DWORD m_trackedChanges;
      int32_t m_xPos;
      int32_t m_yPos;
      int32_t m_xSize;
      int32_t m_ySize;
      uint32_t m_exitCode;
      uint32_t m_style;
      uint32_t m_extendedStyle;
      uint32_t m_minFileChangeDelay;
      uint32_t m_fileChangeDelay;
      const uint32_t m_eventQueueSize;

      HWND m_hwnd;
      HWND m_console;

      bool m_fullscreen;
      bool m_showCursor;
      bool m_lockCursor;
      bool m_relativeMouse;
      bool m_isRunning;

      GamepadHandler m_gamepadHandler;
      MouseHandler m_mouseHandler;
      KeyboardHandler m_keyboardHandler;

      std::atomic<uint32_t> m_headIndex;
      std::atomic<uint32_t> m_tailIndex;
      std::vector<WindowEvent> m_events;

      CReadDirectoryChanges m_monitor;
   };

   void calculateClientRect(uint32_t x, uint32_t y, uint32_t style, uint32_t styleEx, uint32_t& outXSize, uint32_t& outYSize);
}
