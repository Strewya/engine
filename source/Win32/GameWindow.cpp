//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameWindow.h"
	/*** C++ headers ***/
#include <cassert>
	/*** extra headers ***/
#include "Subsystems/Input/Event.h"
#include "Subsystems/Input/KeyCodes.h"
	/*** end headers ***/

namespace Win32
{
	GameWindow::GameWindow(const char* title)
	{
		setTitle(title);
		setClass(title);
		setUsePeekMessage(true);
		setUseWaitMessage(false);
	}

	GameWindow::GameWindow(const String& title)
	{
		setTitle(title);
		setClass(title);
		setUsePeekMessage(true);
		setUseWaitMessage(false);
	}

	LRESULT WINAPI GameWindow::WindowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
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
	/*
		case WM_SETCURSOR:
			POINT cur;
			GetCursorPos(&cur);
			RECT rc;
			GetClientRect(_hwnd, &rc);
			ScreenToClient(_hwnd, &cur);
			if(cur.y > rc.top && cur.y < rc.bottom && cur.x > rc.left && cur.x < rc.right)
			{
				SetCursor( nullptrptr );
				return true;
			}
		break;
		*/
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}