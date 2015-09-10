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
#include "utility/types.h"
#include "window/window_include.h"
/******* extra headers *******/
#include "window/file_change_handler.h"
#include "window/gamepad_handler.h"
#include "window/keyboard_handler.h"
#include "window/mouse_handler.h"
#include "window/read_directory_changes.h"
#include "window/window_message.h"
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
      core_class_scope LRESULT CALLBACK messageRouter(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);

      Window();
      Window(const char* title);
      ~Window();

      LRESULT CALLBACK windowProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);
      bool create();
      void show();
      bool processWin32Messages(CommunicationBuffer* toGame);
      void processCommands(CommunicationBuffer* fromGame, CommunicationBuffer* toGame);

      void setExtendedStyle(u32 style);
      void setStyle(u32 style);
      HWND getWindowHandle() const;
      u32 getStyle() const;
      u32 getExtendedStyle() const;
      const char* getClass() const;
      const char* getTitle() const;
      i32 getExitCode() const;

      void close() const;
      void showMessagebox(const char* title, const char* text) const;
      void resize(u32 x, u32 y);
      void move(i32 x, i32 y);
      void setShowCursor(bool isShown);
      void setLockCursor(bool isLocked);
      void setRelativeCursor(bool isRelative);
      void setFullscreen(bool isFullscreen);
      i32 getPositionX() const;
      i32 getPositionY() const;
      u32 getSizeX() const;
      u32 getSizeY() const;
      void monitorDirectoryForChanges(const char* gameRelativeDirectory);
      void setFileChangeDelay(u32 delay);

   private:
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
      
      HWND m_hwnd;

      bool m_fullscreen;
      bool m_showCursor;
      bool m_lockCursor;
      bool m_relativeMouse;
      bool m_isRunning;

      GamepadHandler m_gamepadHandler;
      MouseHandler m_mouseHandler;
      KeyboardHandler m_keyboardHandler;
      FileChangeHandler m_fileChangeHandler;

      // WinProc specific messages
      enum
      {
         AsyncMsgCount = 8
      };
      u32 m_readAsyncIndex;
      u32 m_writeAsyncIndex;
      WinMsg m_asyncMessages[AsyncMsgCount];
   };

   void calculateClientRect(u32 x, u32 y, u32 style, u32 styleEx, u32& outXSize, u32& outYSize);
}
