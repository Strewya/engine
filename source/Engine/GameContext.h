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
#include "Core/Action/ActionIndex.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
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
		ActionIndex actionIndex;
		ActionUpdater actionQueue;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		b2World physicsWorld;
		float b2ScalingFactor;
		Space activeEntities;

		//this one should stand alone because it needs to be possible to step the logic more than once before rendering
		ActionRptr renderAction;

		GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources);
		virtual ~GameContext();

		void update();

		virtual void destroy();

		/**
		 * This function is called every time the context goes from being inactive to being the active context.
		 * It should create all of the entities that it needs to work properly.
		 */
		virtual void activate() = 0;
		/**
		 * This function is called every time the context goes from being the active one to being inactive.
		 * It should destroy all objects that are created in the ACTIVATE function.
		 * NOTE: Think of a way to keep entities beyond the inactivity boundary.
		 * The onActivate function should take this into consideration when creating that particular entity to check whether it already exists.
		 */
		virtual void deactivate() = 0;
		/**
		 *	This function should be called only once to setup the actions this context is going to use.
		 */
		virtual void registerActions() = 0;
		/**
		 * This function should be called only once to setup the order in which the actions will be updated.
		 */
		virtual void setupActionQueue() = 0;
	protected:
		void input(uint32_t dt);
		void logic(uint32_t dt);
		void render();

		virtual void onInput(uint32_t dt);
		virtual void onLogic(uint32_t dt);
	};
}
