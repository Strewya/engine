//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Window/Window.h>
	/*** C++ headers ***/
#include <algorithm>
#include <cstdlib>
#include <iostream>
	/*** extra headers ***/
#include <Input/KeyCodes.h>
#include <Util/Utility.h>
#include <Window/WindowClass.h>
	/*** end headers ***/

namespace Core
{
	static uint32_t toFileChangeType(DWORD action);
	static Mouse::Keys toMouseKey(uint32_t key, DWORD wParam);


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
		: m_trackedChanges(FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE),
		m_xPos(CW_USEDEFAULT), m_yPos(CW_USEDEFAULT),
		m_xSize(GetSystemMetrics(SM_CXSCREEN)), m_ySize(GetSystemMetrics(SM_CYSCREEN)),
		m_exitCode(0), m_style(0), m_extendedStyle(0), m_minFileChangeDelay(200), m_fileChangeDelay(m_minFileChangeDelay),
		m_hwnd(nullptr),
		m_fullscreen(false), m_showCursor(false), m_isRunning(true),
		m_class(title), m_title(title), m_resourcesDirectory(),
		m_nextFreeSlot(0)
	{
		setFullscreen(m_fullscreen);

		char lpName[128] = {0};
		GetCurrentDirectory(128, lpName);
		m_resourcesDirectory.assign(lpName);
		auto pos = m_resourcesDirectory.find_last_of('\\');
		m_resourcesDirectory.assign(m_resourcesDirectory.substr(0, pos + 1)).append("resources").shrink_to_fit();

		m_monitor.AddDirectory(m_resourcesDirectory.c_str(), true, m_trackedChanges);

		uint8_t count = 32;
		m_fileChanges.reserve(count);
		for(auto i = 0; i < count; ++i)
		{
			 m_fileChanges.emplace_back(FileChangeInfo(i));
		}
	}

	Window::~Window()
	{
#ifdef _DEBUG
		closeConsole();
#endif
		m_monitor.Terminate();
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
#ifdef _DEBUG
		InvalidateRect(m_hwnd, nullptr, true);
#else
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
#endif
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
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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

	bool Window::peekEvent(uint64_t time, WindowEvent& outEvent)
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
		m_timer.update();
		WindowEvent we;
		ZeroMemory(&we, sizeof(we));
		we.m_timestamp = m_timer.getCurMicros();
		return we;
	}

	void Window::processFileChanges()
	{
		std::string file;
		DWORD action;
		while(m_monitor.Pop(action, file))
		{
			if(!file.empty())
			{
				m_timer.update();
				newFileChange(m_timer.getCurMicros(), action, file);
			}
		}

		using std::begin;
		using std::end;
		m_timer.update();
		std::for_each(begin(m_fileChanges), end(m_fileChanges), [&](FileChangeInfo& info)
		{
			if(info.m_state == FileChangeInfo::EVENT_PENDING &&
			   m_timer.getCurMicros() > info.m_timestamp + Time::milisToMicros(m_fileChangeDelay))
			{
				info.m_state = FileChangeInfo::READ_PENDING;
				auto we = newEvent();
				we.m_type = WindowEventType::WE_FILECHANGE;
				we.m_fileChange.m_index = info.m_index;
				m_events.emplace_back(std::move(we));
			}
		});

	}

	void replaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		if(from.empty())
			return;
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	void Window::newFileChange(uint64_t timestamp, DWORD action, const std::string& file)
	{
		using std::begin;
		using std::end;
		
		auto it = std::find_if(begin(m_fileChanges), end(m_fileChanges), [&](const FileChangeInfo& info)
		{
			return info.m_state == FileChangeInfo::EVENT_PENDING && info.m_filename == file && info.m_action == action;
		});
		
		if(it != end(m_fileChanges))
		{
			it->m_timestamp = timestamp;
			return;
		}
		
		auto& info = m_fileChanges[m_nextFreeSlot];
		info.m_timestamp = timestamp;
		info.m_action = action;
		info.m_filename.assign(file);
		info.m_state = FileChangeInfo::EVENT_PENDING;
		m_nextFreeSlot = (m_nextFreeSlot + 1) % m_fileChanges.size();
		if(m_fileChanges[m_nextFreeSlot].m_state == FileChangeInfo::EVENT_PENDING)
		{
			std::cout << "WHOOPS, overwriting a previous file change. This is a BAD THING! Maybe we "
			"should increase the size of our buffer from " << m_fileChanges.size() << "..." << std::endl;
		}
	}
	
	

	bool Window::getChangedFile(uint32_t index, uint32_t& outAction, std::string& outStr)
	{
		if(index < m_fileChanges.size() && m_fileChanges[index].m_state == FileChangeInfo::READ_PENDING)
		{
			outAction = toFileChangeType(m_fileChanges[index].m_action);
			outStr = m_fileChanges[index].m_filename;
			replaceAll(outStr, "\\", "/");
			m_fileChanges[index].m_state = FileChangeInfo::UNUSED;
			m_fileChanges[index].m_action = 0;
			m_fileChanges[index].m_filename.clear();
			m_fileChanges[index].m_timestamp = 0;
			return true;
		}
		return false;
	}
#ifdef _DEBUG
	void Window::openConsole(uint32_t xPos, uint32_t yPos)
	{
		if(!AllocConsole())
			return;
		
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		char name[] { "CoreDebugConsole" };
		SetConsoleTitle(name);
		Sleep(40);
		HWND hConsole = FindWindow(nullptr, name);
		RECT consoleSize;
		GetWindowRect(hConsole, &consoleSize);
		auto x = consoleSize.right - consoleSize.left;
		auto y = consoleSize.bottom - consoleSize.top;
		SetWindowPos(hConsole, 0, xPos, yPos, x, y, 0); //SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW
		SetForegroundWindow(m_hwnd);
	}

	void Window::closeConsole()
	{
		FreeConsole();
	}
#endif
	void Window::setStyle(uint32_t style) { m_style = style; }
	void Window::setExtendedStyle(uint32_t style) { m_extendedStyle = style; }
	void Window::showCursor(bool isShown) { m_showCursor = isShown; }
	void Window::setFileChangeDelay(uint32_t delay) { m_fileChangeDelay = (delay > m_minFileChangeDelay ? delay : m_minFileChangeDelay); }
	
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
	uint32_t		Window::getFileChangeDelay() const { return m_fileChangeDelay; }


	LRESULT WINAPI Window::windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		//here we translate all WM_* messages to the Core::WindowMessage
		bool eventMapped = true;
		LRESULT result = 0;
		const LRESULT notProcessed = -1;
		auto we = newEvent();
		switch(msg)
		{
			case WM_MOUSEMOVE:
			{
				we.m_type = WindowEventType::WE_MOUSEMOVE;
				we.m_mouseMove.m_x = GET_X_LPARAM(lParam);
				we.m_mouseMove.m_y = GET_Y_LPARAM(lParam);
				we.m_mouseMove.m_isRelative = false;
			}
			break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				we.m_type = WindowEventType::WE_KEYBOARDKEY;
				we.m_keyboard.m_keyCode = wParam;
				we.m_keyboard.m_repeat = (uint8_t)LOWORD(lParam);
				we.m_keyboard.m_isDown = true;
				we.m_keyboard.m_previouslyDown = (lParam & (1 << 30)) != 0;
				DEBUG_INFO("Received keypress: ", wParam, ", time:", we.m_timestamp);
			}
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				we.m_type = WindowEventType::WE_KEYBOARDKEY;
				we.m_keyboard.m_keyCode = wParam;
				we.m_keyboard.m_repeat = 1;
				we.m_keyboard.m_isDown = false;
				we.m_keyboard.m_previouslyDown = true;
			}
			break;

			case WM_CHAR:
			{
				we.m_type = WindowEventType::WE_KEYBOARDTEXT;
				we.m_keyboard.m_keyCode = wParam;
				we.m_keyboard.m_repeat = (uint8_t)LOWORD(lParam);
				we.m_keyboard.m_isDown = true;
				we.m_keyboard.m_previouslyDown = false;
			}
			break;

			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				we.m_type = WindowEventType::WE_MOUSEBUTTON;
				we.m_mouseButton.m_clicks = 1;
				we.m_mouseButton.m_isDown = true;
				we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
				we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
				we.m_mouseButton.m_button = toMouseKey(msg, wParam);
				if(we.m_mouseButton.m_button == Mouse::Keys::m_XButton1 || we.m_mouseButton.m_button == Mouse::Keys::m_XButton2)
				{
					result = TRUE;
				}
			}
			break;

			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_XBUTTONDBLCLK:
			{
				we.m_type = WindowEventType::WE_MOUSEBUTTON;
				we.m_mouseButton.m_clicks = 2;
				we.m_mouseButton.m_isDown = true;
				we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
				we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
				we.m_mouseButton.m_button = toMouseKey(msg, wParam);
				if(we.m_mouseButton.m_button == Mouse::Keys::m_XButton1 || we.m_mouseButton.m_button == Mouse::Keys::m_XButton2)
				{
					result = TRUE;
				}
			}
			break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				we.m_type = WindowEventType::WE_MOUSEBUTTON;
				we.m_mouseButton.m_clicks = 0;
				we.m_mouseButton.m_isDown = false;
				we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
				we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
				we.m_mouseButton.m_button = toMouseKey(msg, wParam);
				if(we.m_mouseButton.m_button == Mouse::Keys::m_XButton1 || we.m_mouseButton.m_button == Mouse::Keys::m_XButton2)
				{
					result = TRUE;
				}
			}
			break;

			case WM_MOUSEWHEEL:
			{
				we.m_type = WindowEventType::WE_MOUSEWHEEL;
				we.m_mouseWheel.m_scroll = GET_WHEEL_DELTA_WPARAM(wParam);
				we.m_mouseButton.m_x = GET_X_LPARAM(lParam);
				we.m_mouseButton.m_y = GET_Y_LPARAM(lParam);
			}
			break;

			case WM_CLOSE:
			{
				we.m_type = WindowEventType::WE_CLOSE;
				result = notProcessed;
			}
			break;

			case WM_SETCURSOR:
			{
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
			}
			break;

			case WM_ACTIVATE:
			{
				if(LOWORD(wParam) == WA_INACTIVE)
				{
					we.m_type = WindowEventType::WE_LOSTFOCUS;
				}
				else
				{
					we.m_type = WindowEventType::WE_GAINFOCUS;
				}
				result = notProcessed;
			}
			break;

			default:
			{
				eventMapped = false;
				result = notProcessed;
			}
			break;
		}

		if(eventMapped)
		{
			m_events.emplace_back(we);
		}

		return result == notProcessed ? DefWindowProc(hwnd, msg, wParam, lParam) : result;
	}


	uint32_t toFileChangeType(DWORD action)
	{
		uint32_t returnValue = Core::FILE_BADDATA;
		switch(action)
		{
			case FILE_ACTION_ADDED:
			{
				returnValue = Core::FILE_ADDED;
			}
			break;

			case FILE_ACTION_MODIFIED:
			{
				returnValue = Core::FILE_MODIFIED;
			}
			break;

			case FILE_ACTION_REMOVED:
			{
				returnValue = Core::FILE_REMOVED;
			}
			break;

			case FILE_ACTION_RENAMED_OLD_NAME:
			{
				returnValue = Core::FILE_RENAMED_FROM;
			}
			break;

			case FILE_ACTION_RENAMED_NEW_NAME:
			{
				returnValue = Core::FILE_RENAMED_TO;
			}
			break;

			default:
			break;
		}
		return returnValue;
	}

	Mouse::Keys toMouseKey(uint32_t code, DWORD wParam)
	{
		Mouse::Keys key = Mouse::Keys::m_KeyCount;
		switch(code)
		{
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			{
				key = Mouse::Keys::m_LeftButton;
			}
			break;

			case WM_RBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			{
				key = Mouse::Keys::m_RightButton;
			}
			break;

			case WM_MBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			{
				key = Mouse::Keys::m_MiddleButton;
			}
			break;

			case WM_XBUTTONDBLCLK:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{
				key = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::m_XButton1 : Mouse::m_XButton2;
			}
			break;

			default:
			break;
		}
		return key;
	}
}