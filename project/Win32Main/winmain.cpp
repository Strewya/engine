//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** C++ headers ***/
#define WIN32_LEAN_AND_MEAN 1
#include <cassert>
#include <string>
#include <windows.h>
	/*** extra headers ***/
#include "Engine/Defines.h"
#include "Engine/Engine.h"
#include "GameContextEvents_Pong.h"
#include "Util/Clock.h"
#include "Win32/Window.h"
#include "Win32/WindowClass.h"
	/*** end headers ***/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	Win32::Window window("Snake");
	int result = InitializeWindow(window, false, window.getSizeX()-120, window.getSizeY()-120);
	if(result != ErrorCode::OK)
	{
		return result;
	}

	try
	{
		Core::Engine engine(window);
		engine.getContext("main", Pong::ContextGameplayCreate);
		bool pushed = engine.PushContext("main");
		assert(pushed);
		assert(!engine.PushContext("main"));
		
		//start main loop
		while(window.Update())
		{	
			engine.Loop();
		}

		return window.getExitCode();
	}
	catch(std::exception& ex)
	{
		MessageBox(window.getWindowHandle(), ex.what(), "Exception error", MB_OK);
		return ErrorCode::ExceptionThrown;
	}
}