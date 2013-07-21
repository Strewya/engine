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
	/********************* Triangle *********************/
	struct Triangle : public Core::State<Triangle>
	{
		Util::Vec2 a;
		Util::Vec2 b;
		Util::Vec2 c;
	
		Triangle() {}
		Triangle(const Util::Vec2& a, const Util::Vec2& b, const Util::Vec2& c) : a(a), b(b), c(c) {}
		using State<Triangle>::create;
		static Uptr create(const Util::Vec2& a, const Util::Vec2& b, const Util::Vec2& c)
		{ return Uptr(new Triangle(a,b,c)); }
	};
	

	/********************* TextureHandle *********************/
	struct TextureHandle : public Core::State<TextureHandle>
	{
		InstanceID handle;
	
		TextureHandle() {}
		TextureHandle(InstanceID handle) : handle(handle) {}
		using State<TextureHandle>::create;
		static Uptr create(InstanceID handle)
		{ return Uptr(new TextureHandle(handle)); }
	};
	

	/********************* Circle *********************/
	struct Circle : public Core::State<Circle>
	{
		float radius;
		float width;
		
		Circle() {}
		Circle(float radius, float width) : radius(radius), width(width) {}
		using State<Circle>::create;
		static Uptr create(float radius, float width)
		{ return Uptr(new Circle(radius, width)); }
	};
	
}