//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/b2DebugDraw.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Graphics
{
	b2DebugDraw::b2DebugDraw(IRenderer& renderer)
		: _renderer(&renderer)
	{}

	void b2DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
	}

	void b2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
	}

	void b2DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
	}
	
	void b2DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
	}
	
	void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
	}

	void b2DebugDraw::DrawTransform(const b2Transform& xf)
	{
	}
}