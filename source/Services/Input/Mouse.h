#pragma once
/********************************************
	class:	Mouse
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include "Services/Input/Event.h"
	/*** end header inclusion ***/

namespace Input
{
	class Event;

	class MouseDevice
	{
	public:
		MouseDevice();
		bool handle(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam, std::list<Event>& queue);

	private:
		Event& makeEvent(EventCode type, std::list<Event>& queue);

		uint32_t m_x;
		uint32_t m_y;
		uint32_t m_oldx;
		uint32_t m_oldy;
		
	};
}

