#pragma once
/********************************************
*	class:	RenderQueue
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Vertex.h>
#include <Util/Color.h>
#include <Util/Transform.h>
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
		struct InstanceData
		{
			Transform tf;
			Color col;
		};
		GraphicsSystem* m_graphicsSystem;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<InstanceData> m_instances;
	};
}
