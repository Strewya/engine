//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Graphics/b2DebugDraw.h"
	/*** C++ headers ***/
#include <vector>
	/*** extra headers ***/
#include "Services/Graphics/IRenderer.h"
#include "Services/Graphics/Polygon.h"
	/*** end headers ***/

namespace Graphics
{
	b2DebugDraw::b2DebugDraw(IRenderer& renderer)
		: _renderer(renderer)
	{}

	void b2DebugDraw::setLengthScale(float32 scale)
	{
		_lengthScale = scale;
	}

	float32 b2DebugDraw::getLengthScale() const
	{
		return _lengthScale;
	}

	void b2DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		std::vector<Util::Vec2> polygonData(vertexCount);
		for(int32_t i = vertexCount-1; i >= 0; --i)
		{
			polygonData[i].set(vertices[i].x, vertices[i].y);
		}
		Util::Color c(color.r, color.g, color.b);

		Graphics::Polygon polygon;
		polygon.setColor(c);
		polygon.setSolid(false);
		polygon.setScale(_lengthScale);
		polygon.setManual(polygonData);
		polygon.draw(_renderer);

	}

	void b2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		std::vector<Util::Vec2> polygonData(vertexCount);
		for(int32_t i = vertexCount-1; i >= 0; --i)
		{
			polygonData[i].set(vertices[i].x, vertices[i].y);
		}
		Util::Color c(color.r, color.g, color.b);

		Graphics::Polygon polygon;
		
		polygon.setColor(c);
		polygon.setSolid(true);
		polygon.setEdgeThickness(2);
		polygon.setScale(_lengthScale);
		polygon.setManual(polygonData);
		polygon.draw(_renderer);
	}

	void b2DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		Util::Color c(color.r, color.g, color.b);
		Graphics::Polygon circle;
		circle.setColor(c);
		circle.setSolid(false);
		circle.setScale(_lengthScale);
		circle.setAsCircle(Util::Vec2(center.x, center.y), radius, (uint32_t)radius * 4);
		circle.draw(_renderer);
	}
	
	void b2DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		Util::Color c(color.r, color.g, color.b);
		Graphics::Polygon circle;
		circle.setColor(c);
		circle.setEdgeThickness(2);
		circle.setSolid(true);
		circle.setScale(_lengthScale);
		circle.setAsCircle(Util::Vec2(center.x, center.y), radius, (uint32_t)radius * 4);
		circle.draw(_renderer);
	}
	
	void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		Util::Color c(color.r, color.g, color.b);
		Graphics::Polygon line;
		line.setSolid(false);
		line.setColor(c);
		line.setScale(_lengthScale);
		line.setAsLine(Util::Vec2(p1.x, p1.y), Util::Vec2(p2.x, p2.y));
		line.draw(_renderer);
	}

	void b2DebugDraw::DrawTransform(const b2Transform& xf)
	{	
		b2Color c;
		b2Vec2 p1 = xf.p, p2;
		float scale = 0.4f;
		
		c.Set(1.0f, 0, 0);
		p2 = p1 + scale*xf.q.GetXAxis();
		DrawSegment(p1, p2, c);

		c.Set(0, 0, 1.0f);
		p2 = p1 + scale*xf.q.GetYAxis();
		DrawSegment(p1, p2, c);
	}
}