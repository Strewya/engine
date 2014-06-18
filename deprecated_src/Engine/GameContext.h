#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/Action/ActionRegistry.h>
#include <Core/Entity/EntityFactory.h>
#include <Core/Entity/EntityPool.h>
#include <Core/Space/SpacePool.h>
#include <Engine/MessageSystem.h>
#include <Modules/Input/Service/KeyBindings.h>
#include <Modules/Physics/Service/PhysicsEngine.h>
#include <Util/Time.h>
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

		Input::KeyBindings m_keyBindings;
		MessageSystem m_messenger;

		ActionRegistry m_allOwnedActions;
		ActionUptr m_logic;
		ActionUptr m_render;
		
		GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources, uint64_t updateTime = UPDATE_TIME);
		virtual ~GameContext();

		void createEntity(InstanceID recepient, InstanceID id);
		void destroyEntity(InstanceID id);

		void init();
		void activate();
		void resume();
		
		void setTimeScale(double scale);
		double getTimeScale() const;
		bool isPaused() const;

		void update();
		void render(uint64_t interpolationTime);

		void pause();
		void deactivate();
		void destroy();

	protected:
		

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
		virtual void input();
		virtual void logic();
		virtual void draw(uint64_t interpolationTime);

		//deactivate sequence
		virtual void onDeactivate() = 0;

		//destroy sequence
		virtual void destroyEntities() = 0;
		
	private:
		double m_unpausedTimeScaling;
		double m_currentTimeScaling;
	};
}
