//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Win32/AbstractWindow.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Win32
{
	AbstractWindow::AbstractWindow()
		: _extendedStyle(0), _class("AbstractWindow"), _title("Window"), _xPos(CW_USEDEFAULT), _yPos(CW_USEDEFAULT),
		_xSize(GetSystemMetrics(SM_CXSCREEN)), _ySize(GetSystemMetrics(SM_CYSCREEN)), _hwndParent(nullptr),
		_hInstance(nullptr), _hMenu(nullptr), _style(0), _usePeekMessage(false), _useWaitMessage(false), _exitCode(0)
	{
		setFullscreen(false);
	}
	
	bool AbstractWindow::Create()
	{
		RECT rc = {0,0,_xSize, _ySize};
		AdjustWindowRectEx(&rc, _style, false, _extendedStyle);
		_xSize = rc.right - rc.left;
		_ySize = rc.bottom - rc.top;

		_hwnd = CreateWindowEx(
			_extendedStyle,
			_class.c_str(),
			_title.c_str(),
			_style,
			_xPos,
			_yPos,
			_xSize,
			_ySize,
			_hwndParent,
			_hMenu,
			_hInstance,
			this
			);

		return _hwnd != nullptr;
	}

	void AbstractWindow::Show()
	{
		::ShowWindow(_hwnd, SW_SHOW);
		::UpdateWindow(_hwnd);
	}

	bool AbstractWindow::Update()
	{
		static MSG msg;
		if(!_hwnd)
		{
			MessageBox(nullptr, "Window::HandleMEssage(): The window has not been created yet.", "Runtime error", MB_OK);
			_exitCode = ErrorCode::WindowNotExists;
			return false;
		}

		while(true)
		{
			if((_usePeekMessage)
				? PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)
				: GetMessage(&msg, nullptr, 0, 0))
			{
				if(msg.message == WM_QUIT)
				{
					_exitCode = msg.wParam;
					return false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if(_useWaitMessage)
			{
				WaitMessage();
			}
			else
			{
				return true;
			}
		}
		return true;
	}

	LRESULT CALLBACK AbstractWindow::MessageRouter(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam)
	{
		AbstractWindow* window = nullptr;

		if(msg == WM_NCCREATE)
		{
			window = reinterpret_cast<AbstractWindow*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			::SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<long>(window));
			return ::DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
		window = reinterpret_cast<AbstractWindow*>(::GetWindowLong(hwnd, GWL_USERDATA));

		if(msg == WM_DESTROY)
		{
			PostQuitMessage(0);
			return 0;
		}
		
		if(window != nullptr)
		{
			return window->WindowProc(hwnd, msg, wParam, lParam);
		}
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void AbstractWindow::setFullscreen(bool fs)
	{ 
		_fullscreen = fs;
		if(fs)
		{
			setStyle(WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
			setExtendedStyle(WS_EX_APPWINDOW);
		}
		else
		{
			setStyle(WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);
			setExtendedStyle(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
	}

	void AbstractWindow::setExtendedStyle(ulong style) { _extendedStyle = style; }
	void AbstractWindow::setClass(const String& name) { _class = name; }
	void AbstractWindow::setTitle(const String& name) { _title = name; }
	void AbstractWindow::setClass(const char* name) { _class = name; }
	void AbstractWindow::setTitle(const char* name) { _title = name; }
	void AbstractWindow::setStyle(ulong style) { _style = style; }
	void AbstractWindow::setPosition(int x, int y) { _xPos = x; _yPos = y; }
	void AbstractWindow::setSize(int x, int y) { _xSize = x; _ySize = y; }
	void AbstractWindow::setParent(HWND parent) { _hwndParent = parent; }
	void AbstractWindow::setMenu(HMENU menu) { _hMenu = menu; }
	void AbstractWindow::setInstance(HINSTANCE instance) { _hInstance = instance; }
	void AbstractWindow::setUsePeekMessage(bool peek) { _usePeekMessage = peek; }
	void AbstractWindow::setUseWaitMessage(bool wait) { _useWaitMessage = wait; }

	HWND			AbstractWindow::getWindowHandle() const { return _hwnd; }
	bool			AbstractWindow::getFullscreen() const { return _fullscreen; }
	ulong			AbstractWindow::getExtendedStyle() const { return _extendedStyle; }
	const String&	AbstractWindow::getClass() const { return _class; }
	const String&	AbstractWindow::getTitle() const { return _title; }
	ulong			AbstractWindow::getStyle() const { return _style; }
	int				AbstractWindow::getPositionX() const { return _xPos; }
	int				AbstractWindow::getPositionY() const { return _yPos; }
	int				AbstractWindow::getSizeX() const { return _xSize; }
	int				AbstractWindow::getSizeY() const { return _ySize; }
	HWND			AbstractWindow::getParent() const { return _hwndParent; }
	HMENU			AbstractWindow::getMenu() const { return _hMenu; }
	HINSTANCE		AbstractWindow::getInstance() const { return _hInstance; }
	bool			AbstractWindow::getUsePeekMessage() const { return _usePeekMessage; }
	bool			AbstractWindow::getUseWaitMessage() const { return _useWaitMessage; }
	int				AbstractWindow::getExitCode() const { return _exitCode; }
}