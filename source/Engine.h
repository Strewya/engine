#ifndef _SGEngine_h_
#define _SGEngine_h_
/********************************************
	class:	Engine
	usage:	Primary driver of the game's internals, 
			provides all the services needed by other
			parts of the system
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
#include "windows.h"			//used for HWND handler
#include "SGAppSettings.h"		//used for composition of application settings
#include "SGServiceHolder.h"
	/*** end header inclusion ***/

namespace SG
{
	//fwds
	//end fwds

	class Engine : public ServiceHolder
	{
	public:
		Engine();
		~Engine();
		void Init(HWND hwnd);
		void Loop();
		void Shutdown();

		void Focus();
		void Defocus();

		AppSettings& GetAppSettings();
		HWND GetWindowHandle();
		Entity* GetGame();
		
	private:
		HWND _hwnd;
		TIME _oldFrameTime;
		TIME _accumulator;
		bool _focused;
		shared_ptr<Entity> _game;
		AppSettings _appSettings;
	};
}

#endif //_SGEngine_h_