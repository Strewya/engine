//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Win32/WindowClass.h"
	/*** C++ headers ***/
#include <cstring>
	/*** extra headers ***/
#include "Win32/Window.h"
	/*** end headers ***/

namespace Win32
{
	WindowClass::WindowClass(const char* className)
	{
		this->hInstance		= ::GetModuleHandle(nullptr);
		this->lpszClassName = TEXT(className);
		FillDefaultData();
	}

	WindowClass::WindowClass(const String& className)
	{
		this->hInstance		= ::GetModuleHandle(nullptr);
		this->lpszClassName = TEXT(className.c_str());
		FillDefaultData();
	}

	WindowClass::WindowClass(HINSTANCE hInst, const String& className)
	{
		this->hInstance		= hInst;
		this->lpszClassName = TEXT(className.c_str());
		FillDefaultData();
	}

	WindowClass::WindowClass(HINSTANCE hInst, const char* className)
	{
		this->hInstance		= hInst;
		this->lpszClassName = TEXT(className);
		FillDefaultData();
	}

	void WindowClass::FillDefaultData()
	{
		this->cbSize = sizeof(WNDCLASSEX);
		//fill the structure with info
		this->style			= CS_HREDRAW | CS_VREDRAW;
		this->lpfnWndProc	= (WNDPROC)Window::MessageRouter;
		this->cbClsExtra	= 0;
		this->cbWndExtra	= 0;
		this->hIcon			= LoadIcon(this->hInstance, IDI_APPLICATION);
		this->hCursor		= LoadCursor(this->hInstance, IDC_ARROW);
		this->hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		this->lpszMenuName	= nullptr;
		this->hIconSm		= nullptr;
	}

	ATOM WindowClass::Register()
	{
		return ::RegisterClassEx(this);
	}

	const TCHAR* WindowClass::getClassName() const
	{
		return this->lpszClassName;
	}
}