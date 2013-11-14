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
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
#include "Engine/MessageSystem.h"
#include "Services/Graphics/b2DebugDraw.h"
#include "Services/Input/KeyBindings.h"
#include "Util/Time.h"
	/*** end header inclusion ***/

namespace Core
{
#define FRAMES_PER_SECOND 30ULL
#define UPDATE_TIME 1000000ULL/FRAMES_PER_SECOND

	class ServiceLocator;
	class ResourceLocator;

	enum ContextType
	{
		MENU,
		GAMEPLAY
	};

	class GameContext
	{
	public:
		const ContextType Type;
		ServiceLocator& m_services;
		ResourceLocator& m_resources;
		
		const uint64_t m_updateTime;
		Util::Time m_timer;
		
		EntityPool m_entityPool;
		EntityFactory m_entityFactory;
		
		SpacePool m_spacePool;
		Space m_allEntities;

		b2World m_physicsWorld;
		float m_b2ScalingFactor;
		float m_b2ScalingFactorInv;
		
		Input::KeyBindings m_keyBindings;
		MessageSystem m_messenger;
		
		GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources, uint64_t updateTime = UPDATE_TIME);
		virtual ~GameContext();

		void init();
		void activate();
		void resume();
		
		void setTimeScale(double scale);
		double getTimeScale() const;
		void update();
		void render(uint64_t interpolationTime);

		void pause();
		void deactivate();
		void destroy();

	protected:
		ActionRegistry m_allOwnedActions;
		ActionUptr m_logic;
		ActionUptr m_render;

		/**
		 *	\short Adds an action into the queue specific by the second parameter
		 *	\param[in] action A unique_ptr<Action> to be added to a queue
		 *	\param[in] isLogic If this value is true, the action is added to the queue for logical updates
		 *	\param[in] isRender If this value is true, the action is added to the queue for render updates
		 *	\return A reference to the action that was added
		 */
		ActionRef setupAction(ActionUptr action, bool isLogic, bool isRender);

		/**
		 *	\short Registers all actions that are used by this context. Pure virtual, must be implemented by subclass.
		 */
		virtual void registerActions() = 0;

		/**
			\short Registers all callbacks used by this contexts actions. Pure virtual, must be implemented by subclass.
		*/
		virtual void registerCallbacks() = 0;
		virtual void createEntities() = 0;

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
		double m_timeScaling;
		double m_unpausedTimeScaling;
	};
}
