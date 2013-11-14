#pragma once
/********************************************
	class:	Intent
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	struct Intent
	{
		enum class Type
		{
			Action,
			Range,
			State
		};
		uint32_t msgId;
		Type type;
		InstanceID target;
		union
		{
			bool state;
			double range;
			wchar_t symbol;
		} extraData;

		Intent() : msgId(0), target(0) {}
		Intent(uint32_t _msgId, Type _type, InstanceID _target = 0) : msgId(_msgId), type(_type), target(_target) {}
	};
}
