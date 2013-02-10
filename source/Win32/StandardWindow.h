#ifndef WIN32_STANDARDWINDOW_H_
#define WIN32_STANDARDWINDOW_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <windows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Win32/AbstractWindow.h"
	/*** end header inclusion ***/

namespace Win32
{
	class StandardWindow : public AbstractWindow
	{
	public:
		StandardWindow(const char* title, const char* className);
		StandardWindow(const String& title, const String& className);

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif //WIN32_STANDARDWINDOW_H_