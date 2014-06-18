//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Input/Service/MouseDevice.h>
	/*** C++ headers ***/
#include <windowsx.h>
	/*** extra headers ***/
#include <Modules/Input/Service/KeyCodes.h>
	/*** end headers ***/

namespace Input
{
	Event& MouseDevice::makeEvent(EventCode type, std::deque<Event>& queue)
	{
		queue.emplace_back();
		queue.back().type = type;
		queue.back().device = DeviceCode::Mouse;
		return queue.back();
	}

	bool MouseDevice::handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam, std::deque<Event>& queue)
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
			
			Event& ex = makeEvent(EventCode::Axis, queue);
			ex.axis.code = AxisCode::X;
			ex.axis.value = (double)dx;

			Event& ey = makeEvent(EventCode::Axis, queue);
			ey.axis.code = AxisCode::Y;
			ey.axis.value = (double)dy;

			//SetCursorPos(200,200);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			Event& down = makeEvent(EventCode::Button, queue);
			down.button.code = Mouse::m_LeftButton;
			down.button.down = true;
			break;
		}

		case WM_LBUTTONUP:
		{
			Event& up = makeEvent(EventCode::Button, queue);
			up.button.code = Mouse::m_LeftButton;
			up.button.down = false;
			break;
		}

		case WM_RBUTTONDOWN:
		{
			Event& down = makeEvent(EventCode::Button, queue);
			down.button.code = Mouse::m_RightButton;
			down.button.down = true;
			break;
		}

		case WM_RBUTTONUP:
		{
			Event& up = makeEvent(EventCode::Button, queue);
			up.button.code = Mouse::m_RightButton;
			up.button.down = false;
			break;
		}

		case WM_MBUTTONDOWN:
		{
			Event& down = makeEvent(EventCode::Button, queue);
			down.button.code = Mouse::m_MiddleButton;
			down.button.down = true;
			break;
		}

		case WM_MBUTTONUP:
		{
			Event& up = makeEvent(EventCode::Button, queue);
			up.button.code = Mouse::m_MiddleButton;
			up.button.down = false;
			break;
		}

		case WM_MOUSEWHEEL:
		{
			Event& wheel = makeEvent(EventCode::Axis, queue);
			wheel.axis.code = AxisCode::Z;
			wheel.axis.value = static_cast<double>(GET_WHEEL_DELTA_WPARAM(wparam));
			break;
		}

		//missing are mouse 4 and 5			

		default:
			return false;
		}
		return true;
	}
}