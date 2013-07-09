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
#include "Util/Timer.h"
	/*** end header inclusion ***/

namespace Core
{

#define DeclareState(Name, datatype) struct Name : public State<datatype, Name>\
	{\
	Name() {}\
	Name(const datatype& v) : State(v) {}\
	}
	

	struct defTriangle
	{
		Util::Vec2 a;
		Util::Vec2 b;
		Util::Vec2 c;

		defTriangle() {}
		defTriangle(const Util::Vec2& a, const Util::Vec2& b, const Util::Vec2& c) : a(a), b(b), c(c) {}
	};
	DeclareState(Triangle, defTriangle);

	DeclareState(TextureHandle, InstanceID);
	

	
}