#pragma once
/********************************************
*	class:	RenderQueue
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Vertex.h>
#include <Util/Color.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;
 
   typedef std::function<void(GraphicsSystem& gfx)> RenderCommand;

	class RenderQueue
	{
	public:
		bool init(GraphicsSystem& graphics);
		bool shutdown();

      void enqueueRenderCommand(RenderCommand command);

      void runAndFlushRenderCommands();

	private:
		GraphicsSystem* m_graphicsSystem;
		std::vector<RenderCommand> m_commands;
	};
}
