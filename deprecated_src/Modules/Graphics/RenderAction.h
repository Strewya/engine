#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/Action/Action.h>
#include <Modules/Graphics/Service/RenderingQueue.h>
	/*** end header inclusion ***/

namespace Core
{
	/**************************************************************************
	*		RENDER ACTION
	**************************************************************************/
	class Render : public AtomicAction<Render>
	{
	public:
		void processMessage(GameContext& context, uint32_t msg, InstanceID entity);
		void update(GameContext& context);
		void render(GameContext& context, uint64_t interpolationTime);

	private:
		Graphics::RenderingQueue m_queue;
	};


	/**************************************************************************
	*		MAIN RENDER ACTION
	**************************************************************************/
	class MainRender : public CompositeAction<MainRender>
	{
	public:
		void update(GameContext& context);
		void render(GameContext& context, uint64_t interpolationTime);
	};

}