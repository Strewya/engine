//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Window/Window.h>
	/*** C++ headers ***/
#include <cstdlib>
#include <windef.h>
	/*** extra headers ***/
#include <Input/KeyCodes.h>
#include <Window/WindowClass.h>
	/*** end headers ***/

namespace Core
{
	uint32_t toFileChangeType(DWORD action);

	int initializeWindow(Window& window)
	{
		WindowClass wndClass(window.getClass());

		if (wndClass.registerClass() == 0)
		{
			MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
			return WindowResult::WindowClassRegistrationError;
		}

		if (!window.create())
		{
			MessageBox(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
			return WindowResult::WindowCreationError;
		}

		window.show();
		return WindowResult::OK;
	}


	Window::Window()
		: Window("Window")
	{}

	Window::Window(const std::string& title)
		: Window(title.c_str())
	{}

	Window::Window(const char* title)
		: m_hwnd(nullptr),
		m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
		m_fullscreen(false), m_showCursor(false), m_isRunning(true), m_updateWindow(false),
		m_exitCode(0), m_style(0), m_extendedStyle(0),
		m_xPos(CW_USEDEFAULT), m_yPos(CW_USEDEFAULT),
		m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
		m_class(title), m_title(title), m_resourcesDirectory()
	{
		setFullscreen(m_fullscreen);

		char lpName[128] = {0};
		GetCurrentDirectory(128, lpName);
		m_resourcesDirectory.assign(lpName);
		auto pos = m_resourcesDirectory.find_last_of('\\');
		m_resourcesDirectory.assign(m_resourcesDirectory.substr(0, pos + 1)).append("resources").shrink_to_fit();

		m_monitor.AddDirectory(m_resourcesDirectory.c_str(), true, m_trackedChanges);

		m_dirHandle = CreateFile(
			m_resourcesDirectory.c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			nullptr
			);
		assert(m_dirHandle != INVALID_HANDLE_VALUE);

		
	}

	Window::~Window()
	{
		m_monitor.Terminate();
		CloseHandle(m_dirHandle);
	}

	bool Window::create()
	{
		uint32_t x, y;
		calculateClientRect(x, y);
		HWND hwndParent = nullptr;
		HMENU hMenu = nullptr;
		HINSTANCE hInstance = nullptr;

		m_hwnd = CreateWindowEx(
			m_extendedStyle,
			m_class.c_str(),
			m_title.c_str(),
			m_style,
			m_xPos,
			m_yPos,
			x,
			y,
			hwndParent,
			hMenu,
			hInstance,
			this
			);

		return m_hwnd != nullptr;
	}

	void Window::show()
	{
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}

	void Window::close()
	{
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
	}

	void Window::update()
	{	
		if(!m_hwnd)
		{
			MessageBox(nullptr, "Window::HandleMessage(): The window has not been created yet.", "Runtime error", MB_OK);
			m_exitCode = WindowResult::WindowNotExistsError;
			m_isRunning = false;
			return;
		}
		static MSG msg;
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				m_exitCode = msg.wParam;
				m_isRunning = false;
				return;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(m_updateWindow)
		{
			m_updateWindow = false;
			InvalidateRect(m_hwnd, 0, true);
		}
	}

	void Window::processFileChanges()
	{
		std::string file;
		DWORD action;
		m_timer.update(Time::NORMAL_TIME);
		while(m_monitor.Pop(action, file))
		{
			if(!file.empty())
			{	
				m_fileChangeBuffer.emplace(file, m_timer.getCurMicros());
			}
		}
		std::vector<std::string> deleteThese;
		for(auto& pair : m_fileChangeBuffer)
		{
			//we only want those changes that happened half a second ago
			if(m_timer.getCurMicros() >= (pair.second + Time::microsFromMilis(50)))
			{
				m_fileChanges.emplace_back(pair.first);
				deleteThese.emplace_back(pair.first);
				auto we = newEvent();
				we.m_type = WindowEventType::WINDOW_FILECHANGE;
				we.m_fileChange.m_action = toFileChangeType(action);
				m_events.emplace_back(we);
			}
		}
		for(auto str : deleteThese)
		{
			m_fileChangeBuffer.erase(str);
		}
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

	void Window::calculateClientRect(uint32_t& outXSize, uint32_t& outYSize)
	{
		RECT rc = {0, 0, m_xSize, m_ySize};
		AdjustWindowRectEx(&rc, m_style, false, m_extendedStyle);
		outXSize = rc.right - rc.left;
		outYSize = rc.bottom - rc.top;
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
		
		SetWindowLong(m_hwnd, GWL_STYLE, m_style);
		SetWindowLong(m_hwnd, GWL_EXSTYLE, m_extendedStyle);
		uint32_t x, y;
		calculateClientRect(x, y);
		SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}

	void Window::showMessagebox(const char* title, const char* text)
	{
		MessageBox(m_hwnd, text, title, MB_OK);
	}

	void Window::resize(uint32_t x, uint32_t y)
	{
		m_xSize = x == CW_USEDEFAULT ? GetSystemMetrics(SM_CXSCREEN) : x;
		m_ySize = y == CW_USEDEFAULT ? GetSystemMetrics(SM_CYSCREEN) : y;
		calculateClientRect(x, y);
		SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}

	void Window::move(int32_t xp, int32_t yp)
	{
		m_xPos = xp == CW_USEDEFAULT ? 0 : xp;
		m_yPos = yp == CW_USEDEFAULT ? 0 : yp;
		uint32_t x, y;
		calculateClientRect(x, y);
		SetWindowPos(m_hwnd, 0, m_xPos, m_yPos, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}

	bool Window::peek(uint64_t time, WindowEvent& outEvent)
	{
		bool eventExists = false;
		if(!m_events.empty() && m_events.front().m_timestamp <= time)
		{
			eventExists = true;
			outEvent = m_events.front();
			m_events.pop_front();
		}
		return eventExists;
	}

	WindowEvent Window::newEvent()
	{
		m_timer.update(Time::NORMAL_TIME);
		WindowEvent we;
		ZeroMemory(&we, sizeof(we));
		we.m_timestamp = m_timer.getCurMicros();
		return we;
	}

	void Window::setStyle(uint32_t style) { m_style = style; }
	void Window::setExtendedStyle(uint32_t style) { m_extendedStyle = style; }
	void Window::showCursor(bool isShown) { m_showCursor = isShown; }
	
	const std::string&	Window::getClass() const { return m_class; }
	const std::string&	Window::getTitle() const { return m_title; }
	HWND			Window::getWindowHandle() const { return m_hwnd; }
	uint32_t		Window::getStyle() const { return m_style; }
	uint32_t		Window::getExtendedStyle() const { return m_extendedStyle; }
	int32_t			Window::getPositionX() const { return m_xPos; }
	int32_t			Window::getPositionY() const { return m_yPos; }
	uint32_t		Window::getSizeX() const { return m_xSize; }
	uint32_t		Window::getSizeY() const { return m_ySize; }
	int32_t			Window::getExitCode() const { return m_exitCode; }
	bool			Window::isCursorShown() const { return m_showCursor; }
	bool			Window::isFullscreen() const { return m_fullscreen; }
	bool			Window::isRunning() const { return m_isRunning; }



	LRESULT WINAPI Window::windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		//here we translate all WM_* messages to the Core::WindowMessage
		auto we = newEvent();

		bool eventMapped = true;
		LRESULT result = 0;
		const LRESULT notProcessed = -1;

		switch(msg)
		{
		case WM_MOUSEMOVE:
			we.m_type = WindowEventType::WINDOW_MOUSEMOVE;
			we.m_mouseMove.m_x = GET_X_LPARAM(lParam);
			we.m_mouseMove.m_y = GET_Y_LPARAM(lParam);
			we.m_mouseMove.m_isRelative = false;
			
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			we.m_type = WindowEventType::WINDOW_KEYDOWN;
			we.m_keyboard.m_keyCode = wParam;
			we.m_keyboard.m_repeat = (uint8_t)LOWORD(lParam);
			we.m_keyboard.m_isDown = true;
			we.m_keyboard.m_previouslyDown = (lParam & (1 << 30)) != 0;
		
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			we.m_type = WindowEventType::WINDOW_KEYUP;
			we.m_keyboard.m_keyCode = wParam;
			we.m_keyboard.m_repeat = 1;
			we.m_keyboard.m_isDown = false;
			we.m_keyboard.m_previouslyDown = true;

			break;

		case WM_CHAR:
			we.m_type = WindowEventType::WINDOW_TEXT;
			we.m_keyboard.m_keyCode = wParam;
			we.m_keyboard.m_repeat = (uint8_t)LOWORD(lParam);
			we.m_keyboard.m_isDown = true;
			we.m_keyboard.m_previouslyDown = false;
		
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
			we.m_type = WindowEventType::WINDOW_MOUSEBUTTONDOWN;
			we.m_mouseButton.m_clicks = 1;
			we.m_mouseButton.m_isDown = true;
			we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
			we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
			we.m_mouseButton.m_button = (msg == WM_LBUTTONDOWN ? Mouse::m_LeftButton :
										 (msg == WM_RBUTTONDOWN ? Mouse::m_RightButton :
										 (msg == WM_MBUTTONDOWN ? Mouse::m_MiddleButton :
										 result = TRUE, (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::m_XButton1 : Mouse::m_XButton2))));

			break;

		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
			we.m_type = WindowEventType::WINDOW_MOUSEBUTTONDOWN;
			we.m_mouseButton.m_clicks = 2;
			we.m_mouseButton.m_isDown = true;
			we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
			we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
			we.m_mouseButton.m_button = (msg == WM_LBUTTONDOWN ? Mouse::m_LeftButton :
										 (msg == WM_RBUTTONDOWN ? Mouse::m_RightButton :
										 (msg == WM_MBUTTONDOWN ? Mouse::m_MiddleButton :
										 result = TRUE, (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::m_XButton1 : Mouse::m_XButton2))));

			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
			we.m_type = WindowEventType::WINDOW_MOUSEBUTTONUP;
			we.m_mouseButton.m_clicks = 0;
			we.m_mouseButton.m_isDown = false;
			we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
			we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
			we.m_mouseButton.m_button = (msg == WM_LBUTTONDOWN ? Mouse::m_LeftButton :
										 (msg == WM_RBUTTONDOWN ? Mouse::m_RightButton :
										 (msg == WM_MBUTTONDOWN ? Mouse::m_MiddleButton :
										 result = TRUE, (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::m_XButton1 : Mouse::m_XButton2))));

			break;

		case WM_MOUSEWHEEL:
			we.m_type = WindowEventType::WINDOW_MOUSEWHEEL;
			we.m_mouseWheel.m_scroll = GET_WHEEL_DELTA_WPARAM(wParam);
			we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
			we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);

			break;

		case WM_CLOSE:
			we.m_type = WindowEventType::WINDOW_CLOSE;
			result = notProcessed;
			break;

		case WM_SETCURSOR:
			POINT cur;
			GetCursorPos(&cur);
			RECT rc;
			GetClientRect(m_hwnd, &rc);
			ScreenToClient(m_hwnd, &cur);
			result = notProcessed;
			if(!m_showCursor && cur.y > rc.top && cur.y < rc.bottom && cur.x > rc.left && cur.x < rc.right)
			{
				SetCursor(nullptr);
				result = TRUE;
			}
			eventMapped = false;
			break;

		case WM_ACTIVATE:
			if(LOWORD(wParam) == WA_INACTIVE)
			{
				we.m_type = WindowEventType::WINDOW_LOSTFOCUS;
			}
			else
			{
				we.m_type = WindowEventType::WINDOW_GAINFOCUS;
			}
			result = notProcessed;
			break;


		case WM_PAINT:
		{
						 HBRUSH white = GetStockBrush(WHITE_BRUSH);
						 HBRUSH black = GetStockBrush(BLACK_BRUSH);
						 PAINTSTRUCT ps;

						 auto copy = m_drawStrings;


						 HDC hdc = BeginPaint(hwnd, &ps);
						 
						 RECT bg = {0, 0, m_xSize, m_ySize};
						 FillRect(hdc, &bg, white);
						 
						 uint32_t ofs = 5;
						 uint32_t sz = 10;
						 RECT r;
						 //top left
						 r.left = ofs; r.right = ofs + sz;
						 r.top = ofs; r.bottom = ofs + sz;
						 FillRect(hdc, &r, black);
						 //top right
						 r.left = m_xSize - ofs - sz; r.right = m_xSize - ofs;
						 r.top = ofs; r.bottom = ofs + sz;
						 FillRect(hdc, &r, black);
						 //bottom left
						 r.left = ofs; r.right = ofs + sz;
						 r.top = m_ySize - ofs - sz; r.bottom = m_ySize - ofs;
						 FillRect(hdc, &r, black);
						 //bottom right
						 r.left = m_xSize - ofs - sz; r.right = m_xSize - ofs;
						 r.top = m_ySize - ofs - sz; r.bottom = m_ySize - ofs;
						 FillRect(hdc, &r, black);

						 uint32_t i = 0;
						 for(auto& str : copy)
						 {
							 TextOut(hdc, 50, 30 * i, str.c_str(), str.size());
							 ++i;
						 }
						 

						 EndPaint(hwnd, &ps);
						 eventMapped = false;
						 result = 0;

		}
			break;

		default:
			eventMapped = false;
			result = notProcessed;
		}

		if(eventMapped)
			m_events.emplace_back(we);

		if(result == notProcessed)
			result = DefWindowProc(hwnd, msg, wParam, lParam);
		
		return result;
	}


	uint32_t toFileChangeType(DWORD action)
	{
		uint32_t returnValue = Core::FILE_BADDATA;
		switch(action)
		{
		case FILE_ACTION_ADDED:
			returnValue = Core::FILE_ADDED;
			break;

		case FILE_ACTION_MODIFIED:
			returnValue = Core::FILE_MODIFIED;
			break;

		case FILE_ACTION_REMOVED:
			returnValue = Core::FILE_REMOVED;
			break;

		case FILE_ACTION_RENAMED_OLD_NAME:
			returnValue = Core::FILE_RENAMED_FROM;
			break;

		case FILE_ACTION_RENAMED_NEW_NAME:
			returnValue = Core::FILE_RENAMED_TO;
			break;

		default:
			break;
		}
		return returnValue;
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