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
		this->cbClsExtra	= 0;
		this->cbSize		= sizeof(WNDCLASSEX);
		this->cbWndExtra	= 0;
		this->hbrBackground	= 0; //(HBRUSH)GetStockObject(BLACK_BRUSH);
		this->hCursor		= LoadCursor(nullptr, IDC_ARROW);
		this->hIcon			= LoadIcon(nullptr, IDI_APPLICATION);
		this->hIconSm		= nullptr;
//		this->hInstance		= nullptr; //this is set in ctor
		this->lpfnWndProc	= (WNDPROC)Window::MessageRouter;
//		this->lpszClassName = nullptr; //this is set in ctor
		this->lpszMenuName	= nullptr;
		this->style			= CS_HREDRAW | CS_VREDRAW;
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