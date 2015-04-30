#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <stdint.h>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include "window/window_event.h"
/******* end header inclusion *******/

namespace Core
{
   class Window;

   static const uint32_t USE_MONITOR_RESOLUTION = 0;

   class WindowProxy
   {
   public:
      WindowProxy(Window* window = nullptr);
      bool isValid() const;

      void close() const;
      void showMessagebox(const char* title, const char* text) const;
      void resize(uint32_t x, uint32_t y) const;
      void move(int32_t x, int32_t y) const;
      void showCursor(bool isShown) const;
      void lockCursor(bool isLocked) const;
      void makeMouseRelative(bool isRelative) const;
      void setFullscreen(bool isFullscreen) const;
      int32_t getPositionX() const;
      int32_t getPositionY() const;
      uint32_t getSizeX() const;
      uint32_t getSizeY() const;
      bool isCursorShown() const;
      bool isFullscreen() const;
      bool isRunning() const;
      void openConsole(int32_t xPos, int32_t yPos) const;
      void closeConsole() const;
      void monitorDirectoryForChanges(const char* directory) const;
      void setFileChangeDelay(uint32_t delay) const;
      std::vector<WindowEvent> collectEvents(uint64_t time) const;

      //platform dependant use only
      Window* getRawWindow() const;

   private:
      Window* m_window;
   };
}