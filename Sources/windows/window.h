#pragma once

#include <windows/window_include.h>
#include <core/std.h>

namespace eion
{
  struct window_result
  {
    enum code
    {
      OK = 0,
      WindowClassRegistrationError,
      WindowCreationError,
      InsufficientMemory,
    } Code;
    cstr Message;
  };

  struct window
  {
    cstr Title;

    i32 XPos;
    i32 YPos;
    i32 XSize;
    i32 YSize;
    u32 Style;
    u32 ExtendedStyle;

    HWND Hwnd;

    bool Fullscreen;
    bool ShowCursor;
    bool LockCursor;
    bool RelativeMouse;
  };

  window_result initializeWindow(HINSTANCE Instance, window* Window);
  void showWindow(window* Window);
  LRESULT CALLBACK winProc(HWND Hwnd, u32 Msg, WPARAM WParam, LPARAM LParam);
  void calculateClientRect(u32 X, u32 Y, u32 Style, u32 ExtendedStyle, u32& XSizeOut, u32& YSizeOut);
}
