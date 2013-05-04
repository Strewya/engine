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
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/TextureCache.h"
#include "Util/CompositeTimer.h"
	/*** end header inclusion ***/

namespace Core
{
	class Engine;

	class GameContext
	{
	private:
		Graphics::TextureCache _textureCache;
		Graphics::SpritesheetCache _spritesheetCache;
		
		GameContextEvent _onActivate;
		GameContextEvent _onDeactivate;
		GameContextEvent _onCreate;
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
		ActionUpdater actionMaster;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		ServiceLocator services;
		ResourceLocator resources;
		b2World physicsWorld;

		GameContext(GameContextEvent onCreate, const ServiceLocator& services, const ResourceLocator& resources);
		~GameContext();

		void Create();
		void Destroy();
		void Activate();
		void Deactivate();
		bool Update();
		void setContextEventLogic(EventType type, GameContextEvent function);
	};
}