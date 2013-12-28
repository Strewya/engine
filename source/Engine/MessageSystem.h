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
#include <vector>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	struct Variant
	{
		enum Type
		{
			FLOAT,
			BOOL,
			INT32,
			UINT32,
			STRINGID
		};

		Type m_type;
		uint32_t m_nameID;
		union
		{
			float m_float;
			bool m_bool;
			int32_t m_int32;
			uint32_t m_uint32;
		};
	};

	struct Message
	{
		static const uint32_t MAX_ARGS = 8;
		uint32_t m_nameID;
		Variant args[MAX_ARGS];
		uint8_t m_numArgs;
	};


	class MessageSystem
	{
	public:
		static const InstanceID BROADCAST;

		void subscribe(InstanceID recepient);
		void unsubscribe(InstanceID recepient);

		uint32_t encode(const std::string& messageName);

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
