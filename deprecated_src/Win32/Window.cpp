//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Win32/Window.h>
	/*** C++ headers ***/
#include <windef.h>
	/*** extra headers ***/
#include <Modules/Input/Service/Event.h>
#include <Modules/Input/Service/KeyCodes.h>
#include <Win32/WindowClass.h>
	/*** end headers ***/

namespace Win32
{
	Window::Window()
		: Window("Window")
	{}

	Window::Window(const char* title)
		: m_hwnd(nullptr), m_hwndParent(nullptr), m_hMenu(nullptr), m_hInstance(nullptr),
		  m_usePeekMessage(true), m_useWaitMessage(false), m_fullscreen(false), m_showCursor(false),
		  m_exitCode(0), m_style(0), m_extendedStyle(0),
		  m_xPos(CW_USEDEFAULT), m_yPos(CW_USEDEFAULT),
		  m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
		  m_class(title), m_title(title)
	{
		setFullscreen(m_fullscreen);
	}

	Window::Window(const std::string& title)
		: Window(title.c_str())
	{}

	Window::~Window()
	{}
	
	bool Window::create()
	{
		RECT rc = {0,0,m_xSize, m_ySize};
		AdjustWindowRectEx(&rc, m_style, false, m_extendedStyle);
		m_xSize = rc.right - rc.left;
		m_ySize = rc.bottom - rc.top;

		m_hwnd = CreateWindowEx(
			m_extendedStyle,
			m_class.c_str(),
			m_title.c_str(),
			m_style,
			m_xPos,
			m_yPos,
			m_xSize,
			m_ySize,
			m_hwndParent,
			m_hMenu,
			m_hInstance,
			this
			);

		return m_hwnd != nullptr;
	}

	void Window::show()
	{
		::ShowWindow(m_hwnd, SW_SHOW);
		::UpdateWindow(m_hwnd);
	}

	void Window::shutdown()
	{
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

	bool Window::update()
	{
		static MSG msg;
		if(!m_hwnd)
		{
			MessageBox(nullptr, "Window::HandleMessage(): The window has not been created yet.", "Runtime error", MB_OK);
			m_exitCode = ErrorCode::WindowNotExists;
			return false;
		}

		while(true)
		{
			if((m_usePeekMessage)
				? PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)
				: GetMessage(&msg, nullptr, 0, 0))
			{
				if(msg.message == WM_QUIT)
				{
					m_exitCode = msg.wParam;
					return false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if(m_useWaitMessage)
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

	LRESULT CALLBACK Window::messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
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
			return window->windowProc(hwnd, msg, wParam, lParam);
		}
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::setFullscreen(bool fs)
	{ 
		m_fullscreen = fs;
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
		MessageBox(m_hwnd, text, title, MB_OK);
	}

	void Window::resize()
	{
		SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, m_xSize, m_ySize, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		UpdateWindow(m_hwnd);
	}

	void Window::setExtendedStyle(uint32_t style) { m_extendedStyle = style; }
	void Window::setClass(const std::string& name) { m_class = name; }
	void Window::setTitle(const std::string& name) { m_title = name; }
	void Window::setClass(const char* name) { m_class = name; }
	void Window::setTitle(const char* name) { m_title = name; }
	void Window::setStyle(uint32_t style) { m_style = style; }
	void Window::setPosition(int x, int y) { m_xPos = x; m_yPos = y; }
	void Window::setSize(int x, int y) { m_xSize = x; m_ySize = y; }
	void Window::setParent(HWND parent) { m_hwndParent = parent; }
	void Window::setMenu(HMENU menu) { m_hMenu = menu; }
	void Window::setInstance(HINSTANCE instance) { m_hInstance = instance; }
	void Window::setUsePeekMessage(bool peek) { m_usePeekMessage = peek; }
	void Window::setUseWaitMessage(bool wait) { m_useWaitMessage = wait; }
	void Window::setCursorShow(bool isShown) { m_showCursor = isShown; }
	
	HWND			Window::getWindowHandle() const { return m_hwnd; }
	bool			Window::getFullscreen() const { return m_fullscreen; }
	uint32_t		Window::getExtendedStyle() const { return m_extendedStyle; }
	const std::string&	Window::getClass() const { return m_class; }
	const std::string&	Window::getTitle() const { return m_title; }
	uint32_t		Window::getStyle() const { return m_style; }
	int				Window::getPositionX() const { return m_xPos; }
	int				Window::getPositionY() const { return m_yPos; }
	int				Window::getSizeX() const { return m_xSize; }
	int				Window::getSizeY() const { return m_ySize; }
	HWND			Window::getParent() const { return m_hwndParent; }
	HMENU			Window::getMenu() const { return m_hMenu; }
	HINSTANCE		Window::getInstance() const { return m_hInstance; }
	bool			Window::getUsePeekMessage() const { return m_usePeekMessage; }
	bool			Window::getUseWaitMessage() const { return m_useWaitMessage; }
	int				Window::getExitCode() const { return m_exitCode; }
	bool			Window::isCursorShown() const { return m_showCursor; }

	void Window::registerListener(Listener listener)
	{
		m_listeners.push_back(listener);
	}

	int initializeWindow(Window& window, bool fullscreen, uint32_t width, uint32_t height)
	{
		WindowClass wndClass(window.getClass());
	
		if(wndClass.Register() == 0)
		{
			MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
			return ErrorCode::WindowClassRegistration;
		}

		window.setFullscreen(fullscreen);
		window.setSize(width, height);
	
		if(!window.create())
		{
			MessageBoxA(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
			return ErrorCode::WindowCreation;
		}

		window.show();
		return ErrorCode::OK;
	}

	LRESULT WINAPI Window::windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		if( ! m_listeners.empty() )
		{
			for(auto f : m_listeners)
			{
				if(f(hwnd, msg, wParam, lParam))
				{
					return 0;
				}
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}



/*
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
				m_queue->push_back(e);
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
				m_queue->push_back(e);
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
				m_queue->push_back(e);
				return 0;
			}
			break;
			//----------------------- CHAR -----------------------
			case WM_CHAR:
			{
				Input::Event e;
				e.type = Input::EventType::TextEntered;
				e.text.character = wParam;
				m_queue->push_back(e);
				return 0;
			}	
			break;
			//----------------------- LMB -----------------------
			case WM_LBUTTONDOWN:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonPressed;
				e.mouseButton.button = Input::Mouse::_LeftButton;
				m_queue->push_back(e);
				return 0;
			}
			break;

			case WM_LBUTTONUP:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonReleased;
				e.mouseButton.button = Input::Mouse::_LeftButton;
				m_queue->push_back(e);
				return 0;
			}
			break;
			//----------------------- RMB -----------------------
			case WM_RBUTTONDOWN:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonPressed;
				e.mouseButton.button = Input::Mouse::_RightButton;
				m_queue->push_back(e);
				return 0;
			}
			break;

			case WM_RBUTTONUP:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonReleased;
				e.mouseButton.button = Input::Mouse::_RightButton;
				m_queue->push_back(e);
				return 0;
			}
			break;
			//----------------------- MMB -----------------------
			case WM_MBUTTONDOWN:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonPressed;
				e.mouseButton.button = Input::Mouse::_MiddleButton;
				m_queue->push_back(e);
				return 0;
			}
			break;

			case WM_MBUTTONUP:
			{
				Input::Event e;
				e.type = Input::EventType::MouseButtonReleased;
				e.mouseButton.button = Input::Mouse::_MiddleButton;
				m_queue->push_back(e);
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
				m_queue->push_back(e);
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
				m_queue->push_back(e);
				return TRUE;
			}
			break;

			//----------------------- WHEEL -----------------------
			case WM_MOUSEWHEEL:
			{
				Input::Event e;
				e.type = Input::EventType::MouseWheelMoved;
				e.mouseWheel.delta = HIWORD(wParam);
				m_queue->push_back(e);
				return 0;
			}
			break;
			//----------------------- MOVE -----------------------
			case WM_MOUSEMOVE:
			{
				POINT cursor;
				GetCursorPos(&cursor);
				ScreenToClient(m_hwnd, &cursor);
				Input::Event e;
				e.type = Input::EventType::MouseMoved;
				e.mouseMove.x = cursor.x;
				e.mouseMove.y = cursor.y;
				m_queue->push_back(e);
				return 0;
			}
			break;
			//----------------------- SETCURSOR -----------------------
			case WM_SETCURSOR:
				POINT cur;
				GetCursorPos(&cur);
				RECT rc;
				GetClientRect(m_hwnd, &rc);
				ScreenToClient(m_hwnd, &cur);
				if(!m_showCursor && cur.y > rc.top && cur.y < rc.bottom && cur.x > rc.left && cur.x < rc.right)
				{
					SetCursor( nullptr );
					return true;
				}
			break;
			}
			*/