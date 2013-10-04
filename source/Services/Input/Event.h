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