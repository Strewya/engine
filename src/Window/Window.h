#pragma once
/********************************************
*  class:   Window
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
#include <Window/myWindows.h>
/******* extra headers *******/
#include <Util/Clock.h>
#include <Window/GamepadHandler.h>
#include <Window/KeyboardHandler.h>
#include <Window/MouseHandler.h>
#include <Window/WindowEvent.h>
#include <Window/ReadDirectoryChanges.h>
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

   static const uint32_t MONITOR_RESOLUTION = 0;

   class Window
   {
   public:
      //all platform part
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

      Clock m_clock;

      GamepadHandler m_gamepadHandler;
      MouseHandler m_mouseHandler;
      KeyboardHandler m_keyboardHandler;

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
      uint32_t m_headIndex;
      uint32_t m_tailIndex;
      const uint32_t m_eventQueueSize;

      HWND m_hwnd;

      bool m_fullscreen;
      bool m_showCursor;
      bool m_lockCursor;
      bool m_relativeMouse;
      bool m_isRunning;

      std::string m_class;
      std::string m_title;
      std::string m_resourcesDirectory;

      std::vector<WindowEvent> m_events;


      struct FileChangeInfo
      {
         enum State
         {
            UNUSED,
            EVENT_PENDING,
            READ_PENDING
         };
         FileChangeInfo(uint32_t index)
            : m_timestamp(0), m_action(0), m_index(index), m_filename(), m_state(UNUSED)
         {
         }

         uint64_t m_timestamp;
         DWORD m_action;
         uint32_t m_index;
         State m_state;
         std::string m_filename;
      };

      std::vector<FileChangeInfo> m_fileChanges;
      uint32_t m_nextFreeSlot;
      CReadDirectoryChanges m_monitor;
   };
}
