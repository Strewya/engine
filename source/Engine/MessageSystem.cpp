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
		m_postbox.emplace(recepient, Inbox_t());
	}

	void MessageSystem::unsubscribe(InstanceID recepient)
	{
		m_postbox.erase(recepient);
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
			m_postbox[recepient].emplace_back(msg);
		}
		else
		{
			for(auto it : m_postbox)
			{
				if(it.first != sender)
				{
					it.second.emplace_back(msg);
				}
			}
		}
	}

	bool MessageSystem::consumeMessage(InstanceID recepient, uint32_t& msgId, InstanceID& entity)
	{
		auto it = m_postbox.find(recepient);
		if(it != m_postbox.end())
		{
			if(!it->second.empty())
			{
				msgId = it->second.front().id;
				entity = it->second.front().entity;
				it->second.pop_front();
				return true;
			}
		}
		return false;
	}
}