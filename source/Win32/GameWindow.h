#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <windows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Win32/AbstractWindow.h"
	/*** end header inclusion ***/

namespace Win32
{
	class GameWindow : public AbstractWindow
	{
	public:
		GameWindow(const char* title);
		GameWindow(const String& title);

		LRESULT WINAPI WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

	};
}