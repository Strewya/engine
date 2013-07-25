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
#include "Engine/GameContextEvent.h"
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Core/Action/ActionIndex.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/ITextureCache.h"
#include "Util/CompositeTimer.h"
	/*** end header inclusion ***/

namespace Core
{
	class Engine;

	class GameContext
	{
	private:
		GameContextEvent _onActivate;
		GameContextEvent _onDeactivate;
		GameContextEvent _onDestroy;
		GameContextEvent _onUpdate;
	
	public:
		enum EventType
		{
			OnActivate,
			OnDeactivate,
			OnDestroy,
			OnUpdate
		};

		Util::CompositeTimer timer;
		ActionIndex actionIndex;
		ActionUpdater actionQueue;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		ServiceLocator& services;
		ResourceLocator& resources;
		b2World physicsWorld;

		GameContext(ServiceLocator& services, ResourceLocator& resources);
		~GameContext();

		void destroy();
		void activate();
		void deactivate();
		bool update();
		void setContextEventLogic(EventType type, GameContextEvent function);
	};
}