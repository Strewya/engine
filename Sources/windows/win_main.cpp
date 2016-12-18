#include <windows/window_include.h>

#include <windows/window.h>

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCmd)
{
  MessageBox(nullptr, "Anela glupaco", "ostani s nama", MB_OKCANCEL);

  eion::window Window;
  Window.XPos = 0;
  Window.YPos = 0;
  Window.XSize = 800;
  Window.YSize = 800;
  Window.Fullscreen = false;
  Window.ShowCursor = true;
  Window.LockCursor = false;
  Window.Title = "Tik Tek Tou";
  Window.Style = WS_OVERLAPPEDWINDOW;
  Window.ExtendedStyle = 0;
  eion::window_result Result = eion::initializeWindow(Instance, &Window);
  if( Result.Code != eion::window_result::code::OK )
  {
    MessageBox(nullptr, Result.Message, "Initialization error", MB_OK);
    return Result.Code;
  }

  // things to do:
  // - allocate the memory for the program to run
  // - set up the window proc to pick up mouse clicks
  // - set up the drawing code (BitBlt)
  // - call game code, passing in the general memory, and the output draw buffer that the draw code will take and blit

  return Result.Code;
}