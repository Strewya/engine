#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/State4.h"
#include "Util/Dimensional.h"
	/*** end header inclusion ***/





namespace Core
{
	
#define COMPONENT(Name, datatype) struct Name : public State<datatype, Name>\
	{\
	Name() {}\
	Name(const datatype& v) : State(v) {}\
	}
	

	COMPONENT(Score, uint32_t);
	COMPONENT(Spritesheet, InstanceID);
}