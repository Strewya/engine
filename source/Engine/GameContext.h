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
	enum class ContextType
	{
		MENU,
		GAMEPLAY,
	};

	class GameContext
	{
	public:
		const ContextType Type;
		Util::CompositeTimer timer;
		ActionIndex actionIndex;
		ActionUpdater actionQueue;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		ServiceLocator& services;
		ResourceLocator& resources;
		b2World physicsWorld;
		Space activeEntities;

		GameContext(ContextType Type, ServiceLocator& services, ResourceLocator& resources);
		~GameContext();

		bool update();

		virtual void destroy();
		virtual void onUpdate();
		virtual void activate() = 0;
		virtual void deactivate() = 0;
		virtual void registerActions() = 0;
		virtual void setupActionQueue() = 0;
	};
}