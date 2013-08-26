//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Event.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/
	
namespace Input
{
	bool operator==(const Event& lhs, const Event& rhs)
	{
		if(lhs.type != rhs.type) return false;
		bool isEqual = false;
		switch(lhs.type)
		{
		case EventType::KeyPressed:
			isEqual = lhs.key.code == rhs.key.code &&
					  lhs.key.alt == rhs.key.alt &&
					  lhs.key.control == rhs.key.control &&
					  lhs.key.shift == rhs.key.shift;
			break;

		case EventType::KeyReleased:
			isEqual = lhs.key.code == rhs.key.code &&
					  lhs.key.alt == rhs.key.alt &&
					  lhs.key.control == rhs.key.control &&
					  lhs.key.shift == rhs.key.shift;
			break;

		case EventType::TextEntered:
			isEqual = lhs.text.character == rhs.text.character;
			break;

		case EventType::MouseMoved:
			isEqual = lhs.mouseMove.x == rhs.mouseMove.x &&
					  lhs.mouseMove.y == rhs.mouseMove.y;
			break;

		case EventType::MouseWheelMoved:
			isEqual = lhs.mouseWheel.delta == rhs.mouseWheel.delta;
			break;

		case EventType::MouseButtonPressed:
			isEqual = lhs.mouseButton.button == rhs.mouseButton.button;
			break;

		case EventType::MouseButtonReleased:
			isEqual = lhs.mouseButton.button == rhs.mouseButton.button;
			break;

		case EventType::LostFocus:
			isEqual = true; //no data to compare
			break;

		case EventType::GainedFocus:
			isEqual = true; //no data to compare
			break;

		case EventType::Resized:
			isEqual = true; //there is data for this, but we do not compare it at all because we just use it when the event is received
			break;

		default:
			//noop, we return false at function end
			break;
		}
		return isEqual;
	}
}