#ifndef WIN32_GAMEWINDOW_H_
#define WIN32_GAMEWINDOW_H_
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

namespace SG
{
	class Engine;
}

namespace Win32
{
	class GameWindow : public AbstractWindow
	{
	public:
		GameWindow(const char* title, const char* className, SG::Engine* engine);
		GameWindow(const String& title, const String& className, SG::Engine* engine);

		LRESULT WINAPI WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		SG::Engine* _engine;
	};
}

#endif //WIN32_GAMEWINDOW_H_