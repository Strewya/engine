//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
#include "Engine/Engine.h"
#include "Services/Input/Engine.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources, uint64_t updateTime)
		: Type(type),
		m_services(services), //for ease of access to services
		m_resources(resources), //for ease of access to resources
		m_updateTime(updateTime), //the updateTime for this context, defaults to 30 FPS if not redefined by the derived context class
		m_entityFactory(*this), //not sure if this should be here or more globally, like the Engine object
		m_physicsWorld(b2Vec2(0,0)), //not sure if this should be here or in the Physics Action/Behavior/System
		m_b2ScalingFactor(1),
		m_b2ScalingFactorInv(1/m_b2ScalingFactor),
		m_allEntities(NO_ID), //the currently active entities in the context,
		m_timeScaling(1),
		m_unpausedTimeScaling(1),
		m_logic(new Multiaction),
		m_render(new Multiaction)
	{
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
		if(isLogic)
			m_render->addAction(&actionRef);
		return actionRef;
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
		m_timeScaling = m_unpausedTimeScaling;
	}

	void GameContext::setTimeScale(double scale)
	{
		m_timeScaling = scale;
	}

	double GameContext::getTimeScale() const
	{
		return m_timeScaling;
	}
	
	void GameContext::update()
	{
		m_timer.updateBy(m_updateTime, m_timeScaling);
		m_logic->checkMessages(*this);
		m_logic->update(*this);
	}

	void GameContext::render(uint64_t interpolationTime)
	{
		m_render->render(*this, interpolationTime);
	}

	void GameContext::pause()
	{
		m_unpausedTimeScaling = m_timeScaling;
		m_timeScaling = Util::Time::STOP_TIME;
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




	void GameContext::input(uint32_t dt)
	{}

	void GameContext::logic(uint32_t dt)
	{}
		
	
}
