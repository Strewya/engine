//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** C++ headers ***/
#define WIN32_LEAN_AND_MEAN 1
#include <cassert>
#include <string>
#include <windows.h>
	/*** extra headers ***/
#include "Defines.h"
#include "Engine.h"
#include "GameContextEvents_Pong.h"
#include "Win32/GameWindow.h"
#include "Win32/WindowClass.h"
	/*** end headers ***/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	String className = "Snake";
	Win32::WindowClass wndClass(className);
	
	if(wndClass.Register() == 0)
	{
		MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
		return ErrorCode::WindowClassRegistration;
	}

	Win32::GameWindow gameWindow(wndClass.getClassName(), wndClass.getClassName());
	gameWindow.setFullscreen(false);
	gameWindow.setSize(gameWindow.getSizeX()-30, gameWindow.getSizeY()-120);
	
	if(!gameWindow.Create())
	{
		MessageBoxA(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
		return ErrorCode::WindowCreation;
	}

	gameWindow.Show();

	try
	{
		Core::Engine engine(gameWindow);
		engine.getContext("main", Pong::ContextGameplayCreate);
		assert(engine.PushContext("main"));
		assert(!engine.PushContext("main"));
		
		//start main loop
		while(gameWindow.Update())
		{	
			gClock.FrameStep();
			
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