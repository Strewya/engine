//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/MessageSystem.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	const InstanceID MessageSystem::BROADCAST = 0;

	void MessageSystem::subscribe(InstanceID recepient)
	{
		m_subscribers.emplace(recepient);
	}

	void MessageSystem::unsubscribe(InstanceID recepient)
	{
		m_subscribers.erase(recepient);
	}

	uint32_t MessageSystem::registerMessage(const std::string& messageName)
	{
		auto lcName = lowercase(messageName);
		auto it = std::find(m_messages.begin(), m_messages.end(), lcName);
		if(it == m_messages.end())
		{
			m_messages.push_back(lcName);
			return m_messages.size()-1;
		}
		return std::distance(m_messages.begin(), it);
	}

	void MessageSystem::sendMessage(InstanceID sender, InstanceID recepient, uint32_t msgId, InstanceID entity)
	{
		Msg msg = {msgId, entity};
		if(recepient != BROADCAST)
		{
			m_queue.emplace(recepient, msg);
		}
		else
		{
			std::for_each(m_subscribers.begin(), m_subscribers.end(), [&](InstanceID id){ if(id != sender) m_queue.emplace(id, msg); });
		}
	}

	bool MessageSystem::consumeMessage(InstanceID recepient, uint32_t& msgId, InstanceID& entity)
	{
		auto it = m_queue.find(recepient);
		if(it != m_queue.end())
		{
			msgId = it->second.id;
			entity = it->second.entity;
			m_queue.erase(it);
			return true;
		}
		return false;
	}
}