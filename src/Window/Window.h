#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <atomic>
#include <string>
#include <vector>
/******* common headers *******/
#include <window/window_include.h>
/******* extra headers *******/
#include <window/gamepad_handler.h>
#include <window/keyboard_handler.h>
#include <window/mouse_handler.h>
#include <window/window_event.h>
#include <window/read_directory_changes.h>
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

   static const uint32_t USE_MONITOR_RESOLUTION = 0;

   class Window
   {
   public:
      //generic part
      void showMessagebox(const char* title, const char* text);
      void resize(uint32_t x, uint32_t y);
      void move(int32_t x, int32_t y);

      void showCursor(bool isShown);
      void lockCursor(bool isLocked);
      void makeMouseRelative(bool isRelative);
      void setFullscreen(bool isFullscreen);

      int32_t getPositionX() const;
      int32_t getPositionY() const;
      uint32_t getSizeX() const;
      uint32_t getSizeY() const;
      bool isCursorShown() const;
      bool isFullscreen() const;

      void monitorDirectoryForChanges(const char* directory);
      void setFileChangeDelay(uint32_t delay);
      bool getChangedFile(uint32_t index, uint32_t& outAction, std::string& outStr);
      
      std::vector<WindowEvent> collectEvents(uint64_t time);

      //windows specific, should not be used in game code
      static LRESULT CALLBACK messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

      Window();
      Window(const char* title);
      Window(const std::string& title);
      ~Window();

      LRESULT CALLBACK windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
      bool create();
      void show();
      void update();
      void close();

      void setExtendedStyle(uint32_t style);
      void setStyle(uint32_t style);
      
      HWND getWindowHandle() const;
      uint32_t getStyle() const;
      uint32_t getExtendedStyle() const;
      const std::string& getClass() const;
      const std::string& getTitle() const;
      int32_t getExitCode() const;
      bool isRunning() const;

      void openConsole(int32_t xPos, int32_t yPos);
      void closeConsole();

   protected:
      WindowEvent& newEvent();
      void writeEvent();
      void newFileChange(uint64_t timestamp, DWORD action, const std::string& file);
      void processFileChanges();
      void calculateClientRect(uint32_t& outXSize, uint32_t& outYSize);

      std::string m_class;
      std::string m_title;

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
}
