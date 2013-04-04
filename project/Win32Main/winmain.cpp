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
#include "Win32/GameWindow.h"
#include "Win32/WindowClass.h"
	/*** end headers ***/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	Win32::GameWindow gameWindow("Snake");
	int result = InitializeWindow(gameWindow, false, gameWindow.getSizeX()-120, gameWindow.getSizeY()-120);
	if(result != ErrorCode::OK)
	{
		return result;
	}

	try
	{
		Core::Engine engine(gameWindow);
		engine.getContext("main", Pong::ContextGameplayCreate);
		assert(engine.PushContext("main"));
		assert(!engine.PushContext("main"));
		
		//start main loop
		while(gameWindow.Update())
		{	
			Util::Clock::WallClock().AdvanceTime();
			
			engine.Loop();
		}

		return gameWindow.getExitCode();
	}
	catch(std::exception& ex)
	{
		MessageBox(gameWindow.getWindowHandle(), ex.what(), "Exception error", MB_OK);
		return ErrorCode::ExceptionThrown;
	}
}