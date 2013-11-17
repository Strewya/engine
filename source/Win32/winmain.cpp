//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** C++ headers ***/
#include <cassert>
#include <string>
#include <Win32/myWindows.h>
	/*** extra headers ***/
#include <Engine/Defines.h>
#include <Engine/Engine.h>
#include <Games/GameInit.h>
#include <Win32/Window.h>
#include <Win32/WindowClass.h>
	/*** end headers ***/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
    (void) hInst;
    (void) hPrevInst;
    (void) lpCmdLine;
    (void) nShowCmd;
	Win32::Window window("Snake");
	int result = initializeWindow(window, false, window.getSizeX()-120, window.getSizeY()-120);
	if(result != ErrorCode::OK)
	{
		return result;
	}

	try
	{
		Core::Engine engine(window);
		engine.initializeGame(Game::init);
		
		//start main loop
		while(window.update())
		{	
			engine.loop();
		}

		return window.getExitCode();
	}
	catch(std::exception& ex)
	{
		if(strcmp(ex.what(), "") != 0)
		{
			window.showMessagebox("Exception error", ex.what());
		}
		return ErrorCode::ExceptionThrown;
	}
}
