#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Dimensional.h"
	/*** end header inclusion ***/
	

namespace Util
{
	class Color;
}

namespace Graphics
{
	class IRenderer;

	class Polygon
	{
	public:
		Polygon();

		void setAsCircle(const Util::Vec2& center, float radius, uint32_t numVertices);
		void setAsEllipse(const Util::Vec2& center, float xRadius, float yRadius, uint32_t numVertices);
		void setAsQuad(float halfWidth, float halfHeight);
		void setAsTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3);
		void setAsLine(const Util::Vec2& p1, const Util::Vec2& p2);
		void setManual(const std::vector<Util::Vec2>& data);
		void setManual(const Util::Vec2* vertices, uint32_t numVertices);
		void setColor(const Util::Color& color);
		void setEdgeThickness(float thickness);
		void setSolid(bool isSolid);
		void draw(IRenderer& renderer);

		void setScale(float scale);

	private:
		
		bool m_isSolid;
		float m_thickness;
		Util::Color m_color;
		std::vector<Util::Vec2> m_data;
		Util::Vec2 m_scale;
	};
}
