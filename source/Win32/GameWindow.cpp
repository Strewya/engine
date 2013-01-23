//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameWindow.h"
	/*** C++ headers ***/
#include <cassert>
	/*** extra headers ***/
//#include "Subsystems/Input/SGKeyCodes.h"
	/*** end headers ***/

namespace Win32
{
	GameWindow::GameWindow(const char* title, const char* className, SG::Engine* engine)
	{
		setTitle(title);
		setClass(className);
		_engine = engine;
		assert(_engine);
	}

	GameWindow::GameWindow(const String& title, const String& className, SG::Engine* engine)
	{
		setTitle(title);
		setClass(className);
		_engine = engine;
		assert(_engine);
	}

	LRESULT WINAPI GameWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		bool repeated = false;
		switch(msg)
		{
		case WM_ACTIVATE:
			if(_engine != NULL)
			{
				/*
				if((GetAsyncKeyState(SG::Keys::_Alt)&0x8000) == 0)
				{
					pEngine->getServices().getInput().SetButtonState(SG::Keys::_Alt, false, true);
				}
				if((GetAsyncKeyState(SG::Keys::_Control)&0x8000) == 0)
				{
					pEngine->getServices().getInput().SetButtonState(SG::Keys::_Control, false, true);
				}
				if((GetAsyncKeyState(SG::Keys::_Shift)&0x8000) == 0)
				{
					pEngine->getServices().getInput().SetButtonState(SG::Keys::_Shift, false, true);
				}
				*/
				if(LOWORD(wParam) == WA_INACTIVE)
				{
					//_engine->Defocus();
					int i;
				}
				else
				{
					//_engine->Focus();
					int j;
				}
			}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;
		//----------------------- DOWN -----------------------
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			repeated = (lParam & (1 << 30)) != 0;
			//if(repeat == 1)
				//_engine->getServices().getInput().SetButtonState(wParam, true, repeated);
			return 0;
		break;
		//----------------------- UP -----------------------
		case WM_KEYUP:
		case WM_SYSKEYUP:
			//_engine->getServices().getInput().SetButtonState(wParam, false, true);
			return 0;
		break;
		//----------------------- CHAR -----------------------
		case WM_CHAR:
			/*
			pEngine->getServices().getInput().SetButtonState(wParam, true, LOWORD(lParam) > 0);
			return 0;
			*/
		break;
		//----------------------- LMB -----------------------
		case WM_LBUTTONDOWN:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_LeftButton, true, false);
			return 0;
		break;

		case WM_LBUTTONUP:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_LeftButton, false, true);
			return 0;
		break;
		/*
		case WM_LBUTTONDBLCLK:
			pEngine->getServices().getInput().InjectMouseButtonDoubleClick(SG::Keys::_LeftButton);
			return 0;
		break;*/
		//----------------------- RMB -----------------------
		case WM_RBUTTONDOWN:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_RightButton, true, false);
			return 0;
		break;

		case WM_RBUTTONUP:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_RightButton, false, true);
			return 0;
		break;
		/*
		case WM_RBUTTONDBLCLK:
			pEngine->getServices().getInput().InjectMouseButtonDoubleClick(SG::Keys::_RightButton);
			return 0;
		break;*/
		//----------------------- MMB -----------------------
		case WM_MBUTTONDOWN:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_MiddleButton, true, false);
			return 0;
		break;

		case WM_MBUTTONUP:
			//_engine->getServices().getInput().SetButtonState(SG::Keys::_MiddleButton, false, true);
			return 0;
		break;
		/*
		case WM_MBUTTONDBLCLK:
			pEngine->getServices().getInput().InjectMouseButtonDoubleClick(SG::Keys::_MiddleButton);
			return 0;
		break;*/
		//----------------------- XMB -----------------------
		case WM_XBUTTONDOWN:
		{
			uint btn = GET_XBUTTON_WPARAM(wParam);
			if(btn == XBUTTON1) int j;
				//_engine->getServices().getInput().SetButtonState(SG::Keys::_XButton1, true, false);
			else if(btn == XBUTTON2) int i;
				//_engine->getServices().getInput().SetButtonState(SG::Keys::_XButton2, true, false);
			return TRUE;
		}
		break;

		case WM_XBUTTONUP:
		{
			uint btn = GET_XBUTTON_WPARAM(wParam);
			if(btn == XBUTTON1) 0;
				//_engine->getServices().getInput().SetButtonState(SG::Keys::_XButton1, false, true);
			else if(btn == XBUTTON2) 0;
				//_engine->getServices().getInput().SetButtonState(SG::Keys::_XButton2, false, true);
			return TRUE;
		}
		break;
		/*
		case WM_XBUTTONDBLCLK:
			uint btn = GET_XBUTTON_WPARAM(wParam);
			if(btn == XBUTTON1)
				pEngine->getServices().getInput().InjectMouseButtonDoubleClick(SG::Keys::_XButton1);
			else if(btn == XBUTTON2)
				pEngine->getServices().getInput().InjectMouseButtonDoubleClick(SG::Keys::_XButton2);
			return TRUE;
		break;*/
		//----------------------- WHEEL -----------------------
		case WM_MOUSEWHEEL:
			//_engine->getServices().getInput().SetAxisValue(SG::Axis::_MouseWheel, HIWORD(wParam));
			return 0;
		break;
		//----------------------- MOVE -----------------------
		case WM_MOUSEMOVE:
			POINT cursor;
			GetCursorPos(&cursor);
			ScreenToClient(_hwnd, &cursor);
			//_engine->getServices().getInput().SetAxisValue(SG::Axis::_MouseX, cursor.x);
			//_engine->getServices().getInput().SetAxisValue(SG::Axis::_MouseY, cursor.y);
			return 0;
		break;
	
		case WM_SETCURSOR:
			POINT cur;
			GetCursorPos(&cur);
			RECT rc;
			GetClientRect(_hwnd, &rc);
			ScreenToClient(_hwnd, &cur);
			if(cur.y > rc.top && cur.y < rc.bottom && cur.x > rc.left && cur.x < rc.right)
			{
				SetCursor( NULL );
				return true;
			}
		break;

		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}