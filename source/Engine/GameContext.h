#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Box2D/Box2D.h"
#include "Core/Action/ActionRegistry.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
#include "Engine/IntentSystem.h"
#include "Services/Input/KeyBindings.h"
	/*** end header inclusion ***/

namespace Core
{
	class ServiceLocator;
	class ResourceLocator;

	enum ContextType
	{
		MENU,
		GAMEPLAY,
	};

	class GameContext
	{
	public:
		const ContextType Type;
		ServiceLocator& services;
		ResourceLocator& resources;
		int32_t m_timerId;
		bool m_timerExpired;
		ActionRegistry actionRegistry;
		ActionUpdater actionQueue;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		b2World physicsWorld;
		float b2ScalingFactor;
		Space activeEntities;
		Input::KeyBindings keyBindings;
		IntentSystem intentSystem;

		//this one should stand alone because it needs to be possible to step the logic more than once before rendering
		ActionRptr renderAction;

		GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources);
		virtual ~GameContext();

		void init();
		void activate();
		void update();
		void deactivate();
		void destroy();

	protected:
		//part of the init sequence unique to each game, mjust be overriden
		virtual void registerActions() = 0;
		virtual void registerCallbacks() = 0;
		virtual void createEntities() = 0;

		ActionRef setupAction(ActionUptr action);
		ActionRef setupRenderAction(ActionUptr action);

		//activation sequence
		virtual void onActivate() = 0;

		//update sequence, optional as most of this is done by actions
		virtual void input(uint32_t dt);
		virtual void logic(uint32_t dt);
		//no need for render

		//deactivate sequence
		virtual void onDeactivate() = 0;

		//destroy sequence
		virtual void destroyEntities() = 0;
		
	private:
	};
}
