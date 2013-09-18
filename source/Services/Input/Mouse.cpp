//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Mouse.h"
	/*** C++ headers ***/
#include <windowsx.h>
	/*** extra headers ***/
#include "Services/Input/Event.h"
	/*** end headers ***/

namespace Input
{
	MouseDevice::MouseDevice()
	{
	}

	bool MouseDevice::handle(uint32_t msg, WPARAM wparam, LPARAM lparam, Event& out)
	{
		bool handled = true;
		switch(msg)
		{
		case WM_MOVE:
			out.type = Input::EventType::MouseMoved;
			out.mouseMove.x = GET_X_LPARAM(lparam);
			out.mouseMove.y = GET_Y_LPARAM(lparam);
			break;
		default:
			handled = false;
			break;
		}
		return handled;
	}
}