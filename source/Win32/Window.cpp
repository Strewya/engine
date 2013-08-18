//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Win32/Window.h"
	/*** C++ headers ***/
#include "windef.h"
	/*** extra headers ***/
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
#include "Win32/WindowClass.h"
	/*** end headers ***/

namespace Win32
{
	Window::Window()
		: _hwnd(nullptr), _hwndParent(nullptr), _hMenu(nullptr), _hInstance(nullptr),
		  _usePeekMessage(true), _useWaitMessage(false), _fullscreen(false), _showCursor(false),
		  _exitCode(0), _style(0), _extendedStyle(0),
		  _xPos(CW_USEDEFAULT), _yPos(CW_USEDEFAULT), _xSize(GetSystemMetrics(SM_CXSCREEN)), _ySize(GetSystemMetrics(SM_CYSCREEN)),
		  _class("Window"), _title("Window"),
		  _queue(nullptr)
	{
		setFullscreen(_fullscreen);
	}

	Window::Window(const char* title)
		: _hwnd(nullptr), _hwndParent(nullptr), _hMenu(nullptr), _hInstance(nullptr),
		  _usePeekMessage(true), _useWaitMessage(false), _fullscreen(false), _showCursor(false),
		  _exitCode(0), _style(0), _extendedStyle(0),
		  _xPos(CW_USEDEFAULT), _yPos(CW_USEDEFAULT), _xSize(GetSystemMetrics(SM_CXSCREEN)), _ySize(GetSystemMetrics(SM_CYSCREEN)),
		  _class(title), _title(title),
		  _queue(nullptr)
	{}

	Window::Window(const String& title)
		: _hwnd(nullptr), _hwndParent(nullptr), _hMenu(nullptr), _hInstance(nullptr),
		  _usePeekMessage(true), _useWaitMessage(false), _fullscreen(false), _showCursor(false),
		  _exitCode(0), _style(0), _extendedStyle(0),
		  _xPos(CW_USEDEFAULT), _yPos(CW_USEDEFAULT), _xSize(GetSystemMetrics(SM_CXSCREEN)), _ySize(GetSystemMetrics(SM_CYSCREEN)),
		  _class(title), _title(title),
		  _queue(nullptr)
	{}

	Window::~Window()
	{
	}
	
	bool Window::Create()
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

	void Window::Show()
	{
		::ShowWindow(_hwnd, SW_SHOW);
		::UpdateWindow(_hwnd);
	}

	void Window::Shutdown()
	{
		PostMessage(_hwnd, WM_DESTROY, 0, 0);
	}

	bool Window::Update()
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

	LRESULT CALLBACK Window::MessageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = nullptr;

		if(msg == WM_NCCREATE)
		{
			window = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			::SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<long>(window));
			return ::DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
		window = reinterpret_cast<Window*>(::GetWindowLong(hwnd, GWL_USERDATA));

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

	void Window::setFullscreen(bool fs)
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

	void Window::showMessagebox(const char* title, const char* text)
	{
		MessageBox(_hwnd, text, title, MB_OK);
	}

	void Window::setExtendedStyle(uint32_t style) { _extendedStyle = style; }
	void Window::setClass(const String& name) { _class = name; }
	void Window::setTitle(const String& name) { _title = name; }
	void Window::setClass(const char* name) { _class = name; }
	void Window::setTitle(const char* name) { _title = name; }
	void Window::setStyle(uint32_t style) { _style = style; }
	void Window::setPosition(int x, int y) { _xPos = x; _yPos = y; }
	void Window::setSize(int x, int y) { _xSize = x; _ySize = y; }
	void Window::setParent(HWND parent) { _hwndParent = parent; }
	void Window::setMenu(HMENU menu) { _hMenu = menu; }
	void Window::setInstance(HINSTANCE instance) { _hInstance = instance; }
	void Window::setUsePeekMessage(bool peek) { _usePeekMessage = peek; }
	void Window::setUseWaitMessage(bool wait) { _useWaitMessage = wait; }
	void Window::setEventQueue(std::list<Input::Event>& queue) { _queue = &queue; }
	void Window::setCursorShow(bool isShown) { _showCursor = isShown; }

	HWND			Window::getWindowHandle() const { return _hwnd; }
	bool			Window::getFullscreen() const { return _fullscreen; }
	uint32_t		Window::getExtendedStyle() const { return _extendedStyle; }
	const String&	Window::getClass() const { return _class; }
	const String&	Window::getTitle() const { return _title; }
	uint32_t		Window::getStyle() const { return _style; }
	int				Window::getPositionX() const { return _xPos; }
	int				Window::getPositionY() const { return _yPos; }
	int				Window::getSizeX() const { return _xSize; }
	int				Window::getSizeY() const { return _ySize; }
	HWND			Window::getParent() const { return _hwndParent; }
	HMENU			Window::getMenu() const { return _hMenu; }
	HINSTANCE		Window::getInstance() const { return _hInstance; }
	bool			Window::getUsePeekMessage() const { return _usePeekMessage; }
	bool			Window::getUseWaitMessage() const { return _useWaitMessage; }
	int				Window::getExitCode() const { return _exitCode; }
	bool			Window::isCursorShown() const { return _showCursor; }


	int InitializeWindow(Window& window, bool fullscreen, uint32_t width, uint32_t height)
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

	LRESULT WINAPI Window::WindowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		if(_queue == nullptr)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		switch(msg)
		{
		case WM_ACTIVATE:
		{
			Input::Event e;
			if(LOWORD(wParam) == WA_INACTIVE)
			{
				e.type = Input::EventType::LostFocus;
			}
			else
			{
				e.type = Input::EventType::GainedFocus;
			}
			_queue->push_back(e);
		}
		break;
		
		//----------------------- DOWN -----------------------
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			Input::Event e;
			e.type = Input::EventType::KeyPressed;
			e.key.code = wParam;
			e.key.alt = GetAsyncKeyState(VK_MENU) != 0;
			e.key.shift = GetAsyncKeyState(VK_SHIFT) != 0;
			e.key.control = GetAsyncKeyState(VK_CONTROL) != 0;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- UP -----------------------
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Input::Event e;
			e.type = Input::EventType::KeyReleased;
			e.key.code = wParam;
			e.key.alt = GetAsyncKeyState(VK_MENU) != 0;
			e.key.shift = GetAsyncKeyState(VK_SHIFT) != 0;
			e.key.control = GetAsyncKeyState(VK_CONTROL) != 0;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- CHAR -----------------------
		case WM_CHAR:
		{
			Input::Event e;
			e.type = Input::EventType::TextEntered;
			e.text.character = wParam;
			_queue->push_back(e);
			return 0;
		}	
		break;
		//----------------------- LMB -----------------------
		case WM_LBUTTONDOWN:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonPressed;
			e.mouseButton.button = Input::Mouse::_LeftButton;
			_queue->push_back(e);
			return 0;
		}
		break;

		case WM_LBUTTONUP:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonReleased;
			e.mouseButton.button = Input::Mouse::_LeftButton;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- RMB -----------------------
		case WM_RBUTTONDOWN:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonPressed;
			e.mouseButton.button = Input::Mouse::_RightButton;
			_queue->push_back(e);
			return 0;
		}
		break;

		case WM_RBUTTONUP:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonReleased;
			e.mouseButton.button = Input::Mouse::_RightButton;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- MMB -----------------------
		case WM_MBUTTONDOWN:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonPressed;
			e.mouseButton.button = Input::Mouse::_MiddleButton;
			_queue->push_back(e);
			return 0;
		}
		break;

		case WM_MBUTTONUP:
		{
			Input::Event e;
			e.type = Input::EventType::MouseButtonReleased;
			e.mouseButton.button = Input::Mouse::_MiddleButton;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- XMB -----------------------

		case WM_XBUTTONDOWN:
		{
			uint32_t btn = GET_XBUTTON_WPARAM(wParam);
			Input::Event e;
			e.type = Input::EventType::MouseButtonPressed;
			if(btn == XBUTTON1)
			{
				e.mouseButton.button = Input::Mouse::_XButton1;
			}
			else if(btn == XBUTTON2)
			{
				e.mouseButton.button = Input::Mouse::_XButton2;
			}
			_queue->push_back(e);
			return TRUE;
		}
		break;

		case WM_XBUTTONUP:
		{
			uint32_t btn = GET_XBUTTON_WPARAM(wParam);
			Input::Event e;
			e.type = Input::EventType::MouseButtonReleased;
			if(btn == XBUTTON1)
			{
				e.mouseButton.button = Input::Mouse::_XButton1;
			}
			else if(btn == XBUTTON2)
			{
				e.mouseButton.button = Input::Mouse::_XButton2;
			}
			_queue->push_back(e);
			return TRUE;
		}
		break;

		//----------------------- WHEEL -----------------------
		case WM_MOUSEWHEEL:
		{
			Input::Event e;
			e.type = Input::EventType::MouseWheelMoved;
			e.mouseWheel.delta = HIWORD(wParam);
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- MOVE -----------------------
		case WM_MOUSEMOVE:
		{
			POINT cursor;
			GetCursorPos(&cursor);
			ScreenToClient(_hwnd, &cursor);
			Input::Event e;
			e.type = Input::EventType::MouseMoved;
			e.mouseMove.x = cursor.x;
			e.mouseMove.y = cursor.y;
			_queue->push_back(e);
			return 0;
		}
		break;
		//----------------------- SETCURSOR -----------------------
		case WM_SETCURSOR:
			POINT cur;
			GetCursorPos(&cur);
			RECT rc;
			GetClientRect(_hwnd, &rc);
			ScreenToClient(_hwnd, &cur);
			if(!_showCursor && cur.y > rc.top && cur.y < rc.bottom && cur.x > rc.left && cur.x < rc.right)
			{
				SetCursor( nullptr );
				return true;
			}
		break;
		
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
