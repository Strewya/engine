//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/Box2dDebugDraw.h>
/******* C++ headers *******/
#include <vector>
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Util/Circle.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Vec2.h>
/******* end headers *******/

namespace Core
{
	void Box2dDebugDraw::setLengthScale(float scale)
	{
		m_lengthScale = scale;
	}

	float Box2dDebugDraw::getLengthScale() const
	{
		return m_lengthScale;
	}

	void Box2dDebugDraw::setGraphicsSystem(GraphicsSystem& graphics)
	{
		m_graphics = &graphics;
	}

	void Box2dDebugDraw::clearGraphicsSystem()
	{
		m_graphics = nullptr;
	}

	void Box2dDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		if(m_graphics == nullptr)
			return;

		std::vector<Vec2> polygonData = reverseDirection(vertices, vertexCount);
		Color c(color.r, color.g, color.b);

		Transform t;
		t.scale.set(m_lengthScale, m_lengthScale);
		t.rotation = 0;
		
		drawHollowPolygon(*m_graphics, t, polygonData.data(), vertexCount, c);
	}

	void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		if(m_graphics == nullptr)
			return;

		std::vector<Vec2> polygonData = reverseDirection(vertices, vertexCount);
		Color c(color.r, color.g, color.b);

		Transform t;
		t.scale.set(m_lengthScale, m_lengthScale);
		t.rotation = 0;


		drawSolidPolygon(*m_graphics, t, polygonData.data(), polygonData.size(), c);
	}

	void Box2dDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		if(m_graphics == nullptr)
			return;

		Color c(color.r, color.g, color.b);

		Transform t;
		t.position.set(center.x, center.y);
		t.scale.set(m_lengthScale, m_lengthScale);
		t.rotation = 0;

		drawHollowCircle(*m_graphics, t, radius, 24, c);
	}
	
	void Box2dDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		if(m_graphics == nullptr)
			return;

		Color c(color.r, color.g, color.b);

		Transform t;
		t.position.set(center.x, center.y);
		t.scale.set(m_lengthScale, m_lengthScale);
		t.rotation = 0;

		drawSolidCircle(*m_graphics, t, radius, 24, c);
	}
	
	void Box2dDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		if(m_graphics == nullptr)
			return;

		Color c(color.r, color.g, color.b);
		Vec2 pos[] { Vec2(p1.x, p1.y), Vec2(p2.x, p2.y) };
		Transform t;
		t.scale.set(m_lengthScale, m_lengthScale);
		t.rotation = 0;
		m_graphics->drawMultiline(t, pos, 2, c);
	}

	void Box2dDebugDraw::DrawTransform(const b2Transform& xf)
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

	std::vector<Vec2> Box2dDebugDraw::reverseDirection(const b2Vec2* vertices, int32 vertexCount) const
	{
		std::vector<Vec2> out;
		out.reserve(vertexCount);
		for(int32_t i = vertexCount; i--;)
		{
			out.emplace_back(vertices[i].x, vertices[i].y);
		}
		return out;
	}
}
