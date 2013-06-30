//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Win32/AbstractWindow.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Win32/WindowClass.h"
	/*** end headers ***/

namespace Win32
{
	AbstractWindow::AbstractWindow()
		: _extendedStyle(0), _class("AbstractWindow"), _title("Window"), _xPos(CW_USEDEFAULT), _yPos(CW_USEDEFAULT),
		_xSize(GetSystemMetrics(SM_CXSCREEN)), _ySize(GetSystemMetrics(SM_CYSCREEN)), _hwndParent(nullptr),
		_hInstance(nullptr), _hMenu(nullptr), _style(0), _usePeekMessage(false), _useWaitMessage(false), _exitCode(0),
		_queue(nullptr)
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

	void AbstractWindow::Shutdown()
	{
		PostMessage(_hwnd, WM_DESTROY, 0, 0);
	}

	bool AbstractWindow::Update()
	{
		static MSG msg;
		if(!_hwnd)
		{
			MessageBox(nullptr, "Window::HandleMessage(): The window has not been created yet.", "Runtime error", MB_OK);
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

	LRESULT CALLBACK AbstractWindow::MessageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
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

	void AbstractWindow::setExtendedStyle(uint32_t style) { _extendedStyle = style; }
	void AbstractWindow::setClass(const String& name) { _class = name; }
	void AbstractWindow::setTitle(const String& name) { _title = name; }
	void AbstractWindow::setClass(const char* name) { _class = name; }
	void AbstractWindow::setTitle(const char* name) { _title = name; }
	void AbstractWindow::setStyle(uint32_t style) { _style = style; }
	void AbstractWindow::setPosition(int x, int y) { _xPos = x; _yPos = y; }
	void AbstractWindow::setSize(int x, int y) { _xSize = x; _ySize = y; }
	void AbstractWindow::setParent(HWND parent) { _hwndParent = parent; }
	void AbstractWindow::setMenu(HMENU menu) { _hMenu = menu; }
	void AbstractWindow::setInstance(HINSTANCE instance) { _hInstance = instance; }
	void AbstractWindow::setUsePeekMessage(bool peek) { _usePeekMessage = peek; }
	void AbstractWindow::setUseWaitMessage(bool wait) { _useWaitMessage = wait; }
	void AbstractWindow::setEventQueue(std::list<Input::Event>& queue) { _queue = &queue; }

	HWND			AbstractWindow::getWindowHandle() const { return _hwnd; }
	bool			AbstractWindow::getFullscreen() const { return _fullscreen; }
	uint32_t		AbstractWindow::getExtendedStyle() const { return _extendedStyle; }
	const String&	AbstractWindow::getClass() const { return _class; }
	const String&	AbstractWindow::getTitle() const { return _title; }
	uint32_t		AbstractWindow::getStyle() const { return _style; }
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


	int InitializeWindow(AbstractWindow& window, bool fullscreen, uint32_t width, uint32_t height)
	{
		WindowClass wndClass(window.getClass());
	
		if(wndClass.Register() == 0)
		{
			MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
			return ErrorCode::WindowClassRegistration;
		}

		window.setFullscreen(fullscreen);
		window.setSize(width, height);
	
		if(!window.Create())
		{
			MessageBoxA(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
			return ErrorCode::WindowCreation;
		}

		window.Show();
		return ErrorCode::OK;
	}
}