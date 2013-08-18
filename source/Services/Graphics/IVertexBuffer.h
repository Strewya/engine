#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/
	

namespace Util
{
	class Color;
	class Vec2;
}

namespace Graphics
{
	enum class Topology;

	class IVertexBuffer
	{
	public:
		virtual void copyData(const std::vector<Util::Vec2>& position, const Util::Color& color) = 0;
		virtual void setTopology(Topology t) = 0;
		virtual void bind() = 0;
		virtual void draw() = 0;
	};
}
