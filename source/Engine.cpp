//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Win32/AbstractWindow.h"
	/*** end headers ***/

namespace Core
{
	Engine::Engine(const Win32::AbstractWindow& window) : _window(window), _rendererFactory(window.getInstance(), window.getWindowHandle())
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		Util::Logger& log = Util::GetDefaultLogger();

		log << log.endl;
		log << "*********************************************" << log.endl;
		log	<< "* New program start @ ";
		log << st.wHour << ":" << st.wMinute << ":" << st.wSecond << " on " << st.wDay << "." << st.wMonth << "." << st.wYear << ".  ";
		log << "Application: " << _window.getTitle() << log.endl;
		log << "*********************************************" << log.endl << log.endl;

		/*	read the config file for engine configuration, which needs to include the following:
			* fullscreen status
			* resolution (width and height)
			* display engine (directx by default)
			* audio engine (fmod by default)
		*/
		String renderer = "dx";
		/*
			initialize the subsystems:
			* display
			* audio
			* input
			* script
			* communications
			* persistence
		*/
		_rendererFactory.InitInterface(renderer);
		/*
			initialize the resource caches
			* textures
			* spritesheets
			* meshes
			* sounds
			* scripts
			* TODO: strings
		*/

		/*
			load the game configuration script and execute it
			* the script should create the game contexts that define the game's execution flow

			push the main context on the context stack

			~~end of initialization, if everything passes and no exceptions are thrown, the configuration is OK
		*/
	}

	void Engine::Loop()
	{
		
	}

	void Engine::Shutdown()
	{
		PostMessage(_window.getWindowHandle(), WM_DESTROY, 0, 0);
	}

	const Win32::AbstractWindow& Engine::getWindow() const
	{
		return _window;
	}
}