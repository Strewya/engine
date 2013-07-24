//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/State/State3.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
namespace State_v3
{
	void deleter_noop(InstanceID handle)
	{}


	//****************************************************************************************
	//			STATE IMPLEMENTATION
	//****************************************************************************************
	State::State(const std::type_info& typeInfo)
		: _type(typeInfo)
	{}

	State::~State()
	{}
}
}