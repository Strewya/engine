#include <windows/window.h>

namespace eion
{
  bool createWindow(HINSTANCE Instance, cstr WndClassName, window* Window)
  {
    u32 X, Y;
    calculateClientRect(Window->XSize, Window->YSize, Window->Style, Window->ExtendedStyle, X, Y);
    HWND HwndParent = nullptr;
    HMENU Menu = nullptr;

    Window->Hwnd = CreateWindowEx(
      Window->ExtendedStyle,
      WndClassName,
      Window->Title,
      Window->Style,
      Window->XPos,
      Window->YPos,
      X,
      Y,
      HwndParent,
      Menu,
      Instance,
      Window
    );

    bool Success = Window->Hwnd != nullptr;
    if( Success )
    {
      ::SetWindowLongPtr(Window->Hwnd, GWLP_USERDATA, (long)Window);
    }

    return Success;
  }

  void showWindow(window* Window)
  {
    ShowWindow(Window->Hwnd, SW_SHOW);
    UpdateWindow(Window->Hwnd);
  }

  window_result initializeWindow(HINSTANCE Instance, window* Window)
  {
    WNDCLASSEX WndCls;
    WndCls.cbClsExtra = 0;
    WndCls.cbSize = sizeof(WNDCLASSEX);
    WndCls.cbWndExtra = 0;
    WndCls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndCls.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WndCls.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    WndCls.hIconSm = nullptr;
    WndCls.hInstance = Instance;
    WndCls.lpfnWndProc = (WNDPROC)&winProc;
    WndCls.lpszClassName = "EionWindowClass"; //this is reset in ctor
    WndCls.lpszMenuName = nullptr;
    WndCls.style = CS_HREDRAW | CS_VREDRAW;

    if( RegisterClassEx(&WndCls) == 0 )
    {
      return{window_result::code::WindowClassRegistrationError, "RegisterClassEx failed."};
    }

    if( !createWindow(Instance, WndCls.lpszClassName, Window) )
    {
      return{window_result::WindowCreationError, "Failed to create a window."};
    }

    showWindow(Window);
    return{window_result::code::OK, ""};
  }

  LRESULT processMessage(HWND Hwnd, u32 Msg, WPARAM WParam, LPARAM LParam)
  {
    return ::DefWindowProc(Hwnd, Msg, WParam, LParam);
  }

  LRESULT CALLBACK winProc(HWND Hwnd, u32 Msg, WPARAM WParam, LPARAM LParam)
  {
    window* Window = (window*)(::GetWindowLongPtr(Hwnd, GWLP_USERDATA));

    if( Msg == WM_DESTROY )
    {
      PostQuitMessage(0);
      return 0;
    }

    if( Window != nullptr )
    {
      return processMessage(Hwnd, Msg, WParam, LParam);
    }
    return ::DefWindowProc(Hwnd, Msg, WParam, LParam);
  }

  void calculateClientRect(u32 X, u32 Y, u32 Style, u32 ExtendedStyle, u32& XSizeOut, u32& YSizeOut)
  {
    RECT Rect = {0, 0, X, Y};
    AdjustWindowRectEx(&Rect, Style, false, ExtendedStyle);
    XSizeOut = Rect.right - Rect.left;
    YSizeOut = Rect.bottom - Rect.top;
  }

#if 0


  Window::Window()
    : Window("Window")
  {}

  Window::Window(const char* title) :
    m_class(title), m_title(title),
    m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
    m_xPos(0), m_yPos(0),
    m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
    m_exitCode(0), m_style(0), m_extendedStyle(0),
    m_hwnd(nullptr),
    m_fullscreen(false), m_showCursor(false), m_lockCursor(false), m_relativeMouse(false),
    m_isRunning(true),
    m_readAsyncIndex(0), m_writeAsyncIndex(1)
  {}

  Window::~Window()
  {
    setLockCursor(false);
    setShowCursor(true);
    setRelativeCursor(false);
  }


  void Window::show()
  {
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
  }

  bool Window::processWin32Messages(CommunicationBuffer* toGame)
  {
    MSG msg;
    while( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
    {
      if( msg.message == WM_QUIT )
      {
        m_exitCode = (u32)msg.wParam;
        return false;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if( msg.message == WM_CLOSE )
      {
        WinMsg we{};
        we.type = WinMsgType::Close;
        toGame->writeEvent(we);
      }
      //write the mouse event to the input queue that goes to the game
      m_mouseHandler.handle(toGame, msg.message, msg.wParam, msg.lParam);
      m_keyboardHandler.handle(toGame, msg.message, msg.wParam, msg.lParam);
    }
    m_gamepadHandler.handle(toGame);
    m_fileChangeHandler.handle(toGame);
    while( (m_readAsyncIndex + 1) != m_writeAsyncIndex )
    {
      ++m_readAsyncIndex;
      toGame->writeEvent(m_asyncMessages[m_readAsyncIndex % AsyncMsgCount]);
    }
    return true;
  }

  void Window::processCommands(CommunicationBuffer* fromGame, CommunicationBuffer* toGame)
  {
    core::WinMsg cmd;
    while( fromGame->peek(cmd) )
    {
      switch( cmd.type )
      {
        case core::WinMsgType::LockCursor:
        {
          setLockCursor(cmd.lockCursor);
        } break;
        case core::WinMsgType::ShowCursor:
        {
          setShowCursor(cmd.showCursor);
        } break;
        case core::WinMsgType::RelativeCursor:
        {
          setRelativeCursor(cmd.relativeCursor);
        } break;
        case core::WinMsgType::Fullscreen:
        {
          setFullscreen(cmd.fullscreen);
        } break;
        case core::WinMsgType::Size:
        {
          resize(cmd.screen.x, cmd.screen.y);
          WinMsg msg{};
          msg.type = core::WinMsgType::Size;
          msg.screen.x = getSizeX();
          msg.screen.y = getSizeY();
          toGame->writeEvent(msg);
        } break;
        case core::WinMsgType::Position:
        {
          move(cmd.screen.x, cmd.screen.y);
        } break;
        case core::WinMsgType::FileChange:
        {
          monitorDirectoryForChanges(cmd.fileChange.name);
        } break;
        case core::WinMsgType::Close:
        {
          close();
        } break;
      }
    }
  }

  void Window::setStyle(u32 style)
  {
    m_style = style;
  }

  void Window::setExtendedStyle(u32 style)
  {
    m_extendedStyle = style;
  }

  const char* Window::getClass() const
  {
    return m_class;
  }

  const char* Window::getTitle() const
  {
    return m_title;
  }

  HWND Window::getWindowHandle() const
  {
    return m_hwnd;
  }

  u32 Window::getStyle() const
  {
    return m_style;
  }

  u32 Window::getExtendedStyle() const
  {
    return m_extendedStyle;
  }

  i32 Window::getExitCode() const
  {
    return m_exitCode;
  }

  void Window::close() const
  {
    InvalidateRect(m_hwnd, nullptr, true);
    PostMessage(m_hwnd, WM_CLOSE, 0, 0);
  }

  void Window::showMessagebox(const char* title, const char* text) const
  {
    MessageBox(nullptr, text, title, MB_OK);
  }

  void Window::resize(u32 x, u32 y)
  {
    m_xSize = (x == CW_USEDEFAULT || x == 0) ? GetSystemMetrics(SM_CXSCREEN) : x;
    m_ySize = (y == CW_USEDEFAULT || y == 0) ? GetSystemMetrics(SM_CYSCREEN) : y;
    calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
    SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  }

  void Window::move(i32 xp, i32 yp)
  {
    m_xPos = xp == CW_USEDEFAULT ? 0 : xp;
    m_yPos = yp == CW_USEDEFAULT ? 0 : yp;
    u32 x, y;
    calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
    SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  }

  void Window::setShowCursor(bool isShown)
  {
    m_showCursor = isShown;
  }

  void Window::setLockCursor(bool isLocked)
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

  void Window::setRelativeCursor(bool isRelative)
  {
    m_mouseHandler.setRelativeMouseMove(isRelative, m_xSize / 2, m_ySize / 2);
  }

  void Window::setFullscreen(bool isFullscreen)
  {
    m_fullscreen = isFullscreen;
    if( isFullscreen )
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
    u32 x, y;
    calculateClientRect(m_xSize, m_ySize, m_style, m_extendedStyle, x, y);
    SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  }

  i32 Window::getPositionX() const
  {
    return m_xPos;
  }

  i32 Window::getPositionY() const
  {
    return m_yPos;
  }

  u32 Window::getSizeX() const
  {
    return m_xSize;
  }

  u32 Window::getSizeY() const
  {
    return m_ySize;
  }

  void Window::monitorDirectoryForChanges(const char* gameRelativeDirectory)
  {
    enum
    {
      MaxPathLength = 256,
    };
    char lpName[MaxPathLength] = {0};
    u32 written = GetCurrentDirectory(MaxPathLength, lpName);
    sprintf(lpName + written, "\\%s", gameRelativeDirectory);

    m_fileChangeHandler.addDirectory(lpName, m_trackedChanges);
  }

  LRESULT CALLBACK Window::messageRouter(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam)
  {
    Window* window = nullptr;

    if( msg == WM_NCCREATE )
    {
      window = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
      ::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<long>(window));
      return ::DefWindowProc(hwnd, msg, wParam, lParam);
    }

    window = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if( msg == WM_DESTROY )
    {
      PostQuitMessage(0);
      return 0;
    }

    if( window != nullptr )
    {
      return window->windowProc(hwnd, msg, wParam, lParam);
    }
    return ::DefWindowProc(hwnd, msg, wParam, lParam);
  }

  LRESULT WINAPI Window::windowProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam)
  {
    //here we translate window related WM_* messages to Core::WindowEvent
    LRESULT result = 0;
    switch( msg )
    {
      case WM_ACTIVATE:
      {
        if( LOWORD(wParam) == WA_INACTIVE )
        {
          m_asyncMessages[m_writeAsyncIndex%AsyncMsgCount].type = WinMsgType::LostFocus;
        }
        else
        {
          m_asyncMessages[m_writeAsyncIndex%AsyncMsgCount].type = WinMsgType::GainFocus;
        }
        ++m_writeAsyncIndex;
      } break;

      case WM_SETCURSOR:
      {
        POINT cur;
        GetCursorPos(&cur);
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        ScreenToClient(m_hwnd, &cur);
        if( !m_showCursor && cur.y >= rc.top && cur.y <= rc.bottom && cur.x >= rc.left && cur.x <= rc.right )
        {
          SetCursor(nullptr);
          result = TRUE;
        }
        else
        {
          result = DefWindowProc(hwnd, msg, wParam, lParam);
        }
      } break;

      default:
      {
        result = DefWindowProc(hwnd, msg, wParam, lParam);
      }
        break;
    }

    return result;
  }

  void calculateClientRect(u32 x, u32 y, u32 style, u32 styleEx, u32& outXSize, u32& outYSize)
  {
    RECT rc = {0, 0, x, y};
    AdjustWindowRectEx(&rc, style, false, styleEx);
    outXSize = rc.right - rc.left;
    outYSize = rc.bottom - rc.top;
  }
#endif
}