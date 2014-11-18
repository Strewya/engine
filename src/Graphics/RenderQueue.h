#pragma once
/********************************************
*	class:	RenderQueue
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;

	class RenderQueue
	{
	public:
		bool init(GraphicsSystem& graphics);
		bool shutdown();

		void drawLine();
		void drawMultiline();
		void drawRect();
		void drawCircle();
		void drawPolygon();
		void drawSolidRect();
		void drawSolidCircle();
		void drawSolidPolygon();
		void drawTexturedQuad();
		void drawText();

		void drawCustomData();

		void setTransparency();
		void setCulling();



	private:
		GraphicsSystem* m_graphicsSystem;

	};
}
