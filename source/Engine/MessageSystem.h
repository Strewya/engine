#pragma once
/********************************************
	class:	SGEngine_code_files
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
#include <deque>
#include <unordered_map>
#include <unordered_set>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	class MessageSystem
	{
	public:
		static const InstanceID BROADCAST;

		void subscribe(InstanceID recepient);
		void unsubscribe(InstanceID recepient);

		uint32_t registerMessage(const std::string& messageName);

		void sendMessage(InstanceID sender, InstanceID recepient, uint32_t msgId, InstanceID entity);
		bool consumeMessage(InstanceID recepient, uint32_t& msgId, InstanceID& entity);
		
	private:
		struct Msg
		{
			uint32_t id;
			InstanceID entity;
		};
		typedef std::deque<Msg> Inbox_t;
		typedef std::unordered_map<InstanceID, Inbox_t> Postbox_t;
		typedef std::vector<std::string> MessageCodes_t;

		Postbox_t m_postbox;
		MessageCodes_t m_messages;
	};
}
