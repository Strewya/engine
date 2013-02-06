//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Win32/StandardWindow.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Win32
{
	StandardWindow::StandardWindow(const char* title, const char* className)
	{
		setTitle(title);
		setClass(className);
	}

	StandardWindow::StandardWindow(const String& title, const String& className)
	{
		setTitle(title);
		setClass(className);
	}

	LRESULT WINAPI StandardWindow::WindowProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{

		default:
			return ::DefWindowProcA(hwnd, msg, wParam, lParam);
		}
	}
}