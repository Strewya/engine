//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Mouse.h"
	/*** C++ headers ***/
#include <windowsx.h>
	/*** extra headers ***/
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
	/*** end headers ***/

namespace Input
{
	MouseDevice::MouseDevice()
	{
	}

	bool MouseDevice::handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam, std::list<Event>& queue)
	{
		switch(msg)
		{
		case WM_MOVE:
		{
			m_oldx = m_x;
			m_oldy = m_y;
			m_x = GET_X_LPARAM(lparam);
			m_y = GET_Y_LPARAM(lparam);
			auto dx = m_x - m_oldx;
			auto dy = m_y - m_oldy;
			
			//absolute mouse pos
			queue.emplace_back();
			Event& e = queue.back();
			e.type = EventType::MouseMoved;
			e.mouseMove.relative = false;
			e.mouseMove.x = m_x;
			e.mouseMove.y = m_y;

			//relative movement
			queue.emplace_back();
			Event& f = queue.back();
			f.type = EventType::MouseMoved;
			f.mouseMove.relative = true;
			f.mouseMove.x = dx;
			f.mouseMove.y = dy;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			queue.emplace_back();
			Event& e = queue.back();
			e.type = EventType::MouseButtonPressed;
			e.mouseButton.button = Mouse::_LeftButton;
			break;
		}

		case WM_LBUTTONUP:
		{
			queue.emplace_back();
			Event& e = queue.back();
			e.type = EventType::MouseButtonReleased;
			e.mouseButton.button = Mouse::_LeftButton;
			break;
		}

		default:
			return false;
		}
		return true;
	}
}