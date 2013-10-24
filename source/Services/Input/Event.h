#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Input
{
	enum class DeviceCode
	{
		Keyboard,
		Mouse,
	};

	enum class EventCode
	{
		Button,
		Axis,
		Text,
	};

	enum class AxisCode
	{
		X,
		Y,
		Z,
	};

	class Event
	{
	public:
		Event() {}
		Event(DeviceCode device, EventCode type, uint32_t buttonCode, bool buttonState)
			: device(device), type(type)
		{ button.code = buttonCode; button.down = buttonState; }

		Event(DeviceCode device, EventCode type, AxisCode axisCode, float minTolerance)
			: device(device), type(type)
		{ axis.code = axisCode; axis.value = minTolerance; }
		
		Event(DeviceCode device, EventCode type, wchar_t symbol)
			: device(device), type(type)
		{ text = symbol; }
		
		DeviceCode device;
		EventCode type;
		//timestamp needed here
		union
		{
			struct
			{
				uint32_t code;
				bool down;
			} button;
			struct
			{
				AxisCode code;
				float value;
			} axis;
			wchar_t text;
		};
	};
}