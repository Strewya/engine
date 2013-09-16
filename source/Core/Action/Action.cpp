//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Util/Time.h"
	/*** end headers ***/


namespace Core
{
	Action::Action(InstanceID type, GameContext& context)
		: uid(type), m_context(context), m_timerExpired(false), m_timerId(INVALID_ID)
	{
		m_timerId = context.services.getClock().createAccumulator(context.services.getClock().getTimePerFrame(), [&](){m_timerExpired=true;});
		context.services.getClock().slaveTimers(context.m_timerId, m_timerId);
	}

	Action::~Action()
	{
		m_context.services.getClock().deleteTimer(m_timerId);
	}

	void Action::update()
	{
		if(m_timerExpired)
		{
			auto& timer = m_context.services.getClock().getTimer(m_timerId);
			if(timer.m_currentTime >= timer.m_targetTime)
			{
				timer.m_currentTime -= timer.m_targetTime;
				onUpdate( static_cast<float>( timer.m_targetTime )/1000.0f );
			}
			if(timer.m_currentTime < timer.m_targetTime)
			{
				m_timerExpired = false;
			}
		}
	}

	bool Action::registerEntity(InstanceID id)
	{
		return validateEntity(id) && m_entities.insert(id).second;
	}

	bool Action::unregisterEntity(InstanceID id)
	{
		return m_entities.erase(id) > 0;
	}

	bool Action::validateEntity(InstanceID id)
	{
		return m_context.entityPool.isAlive(id) && 
			validateEntity( m_context.entityPool.getInstanceRef(id) );
	}

	void Action::registerCallbacks()
	{}
}