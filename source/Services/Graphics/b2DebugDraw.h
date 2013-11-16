#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Box2D/Box2D.h>
	/*** end header inclusion ***/

namespace Graphics
{
	class IRenderer;

	class b2DebugDraw : public b2Draw
	{
	public:
		void setRenderer(IRenderer& renderer);
		void clearRenderer();

		void setLengthScale(float scale);
		float getLengthScale() const;
		
		/// Draw a closed polygon provided in CCW order.
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a solid closed polygon provided in CCW order.
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a circle.
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	
		/// Draw a solid circle.
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	
		/// Draw a line segment.
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

		/// Draw a transform. Choose your own length scale.
		/// @param xf a transform.
		void DrawTransform(const b2Transform& xf);

	private:
		IRenderer* m_renderer;
		float m_lengthScale;
	};
}