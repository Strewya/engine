#pragma once
/********************************************
	class:	Mouse
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Input
{
	class Event;

	class MouseDevice
	{
	public:
		MouseDevice();
		bool handle(uint32_t msg, WPARAM wParam, LPARAM lParam, Event& out);

	private:
		
	};
}

