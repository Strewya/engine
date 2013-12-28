//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Engine/GameContext.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/Action/Action.h>
#include <Modules/Input/Service/InputEngine.h>
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources, uint64_t updateTime)
		: Type(type),
		m_services(services), //for ease of access to services
		m_resources(resources), //for ease of access to resources
		m_updateTime(updateTime), //the updateTime for this context, defaults to 30 FPS if not redefined by the derived context class
		m_entityFactory(*this), //not sure if this should be here or more globally, like the Engine object
		m_allEntities(NO_ID), //the currently active entities in the context,
		m_logic(new Multiaction),
		m_render(new Multiaction),
		m_unpausedTimeScaling(1),
		m_currentTimeScaling(m_unpausedTimeScaling)
	{
		m_entityPool.registerDestructionCallback([&](InstanceID id)
		{
			m_messenger.sendMessage(0, MessageSystem::BROADCAST, m_messenger.encode("destroy_entity"), id);
		});
	}

	GameContext::~GameContext()
	{
	}

	ActionRef GameContext::setupAction(ActionUptr action, bool isLogic, bool isRender)
	{
		assert(isLogic || isRender);

		ActionRef actionRef = m_allOwnedActions.addAction(std::move(action));
		if(isLogic)
			m_logic->addAction(&actionRef);
		if(isRender)
			m_render->addAction(&actionRef);
		actionRef.initialize(*this);
		return actionRef;
	}

	void GameContext::createEntity(InstanceID recepient, InstanceID id)
	{
		m_allEntities.addEntity(id);
		m_messenger.sendMessage(0, recepient, m_messenger.encode("add_entity"), id);
	}

	void GameContext::destroyEntity(InstanceID id)
	{
		m_allEntities.removeEntity(id);
		m_messenger.sendMessage(0, m_messenger.BROADCAST, m_messenger.encode("destroy_entity"), id);
	}

	// event methods
	void GameContext::init()
	{
		registerActions();
		registerCallbacks();
		createEntities();
	}

	void GameContext::activate()
	{
		onActivate();
		resume();
	}

	void GameContext::resume()
	{
		m_currentTimeScaling = m_unpausedTimeScaling;
	}

	void GameContext::setTimeScale(double scale)
	{
		m_unpausedTimeScaling = scale;
		if(!isPaused())
		{
			m_currentTimeScaling = scale;
		}
	}

	bool GameContext::isPaused() const
	{
		return m_currentTimeScaling == Util::Time::STOP_TIME;
	}

	double GameContext::getTimeScale() const
	{
		return m_currentTimeScaling;
	}
	
	void GameContext::update()
	{
		m_timer.updateBy(m_updateTime, m_currentTimeScaling);
		m_logic->checkMessages(*this);
		m_logic->update(*this);
		
		input();
		logic();
	}

	void GameContext::render(uint64_t interpolationTime)
	{
		m_render->render(*this, interpolationTime);
		
		draw(interpolationTime);
	}

	void GameContext::pause()
	{
		m_currentTimeScaling = Util::Time::STOP_TIME;
	}

	void GameContext::deactivate()
	{
		pause();
		onDeactivate();
	}

	void GameContext::destroy()
	{
		destroyEntities();
	}




	void GameContext::input()
	{}

	void GameContext::logic()
	{}
		
	void GameContext::draw(uint64_t interpolationTime)
    {
        (void)interpolationTime; /* touch */
    }
}
