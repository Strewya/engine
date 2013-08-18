#pragma once

namespace Graphics
{
	enum class RS_Culling
	{
		None,
		Clockwise,
		CounterClockwise,
	};

	enum class RS_Fillmode
	{
		Point,
		Wireframe,
		Solid,
	};

	enum class Topology
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
	};
}