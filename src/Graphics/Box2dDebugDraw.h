#pragma once
/********************************************
*	class:	Box2dDebugDraw
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
#include <Box2D/Box2D.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;
   template<typename T>
   class Vec2;
   typedef Vec2<float> Vec2f;

	class Box2dDebugDraw : public b2Draw
	{
	public:
		void setLengthScale(float scale);
		float getLengthScale() const;
		void setGraphicsSystem(GraphicsSystem& graphics);
		void clearGraphicsSystem();

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
		float m_lengthScale;
		GraphicsSystem* m_graphics;

		std::vector<Vec2f> reverseDirection(const b2Vec2* vertices, int32 vertexCount) const;
	};
}