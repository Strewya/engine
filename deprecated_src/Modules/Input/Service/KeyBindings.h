#pragma once
/********************************************
	class:	KeyBindingSystem
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
#include <map>
#include <unordered_map>
	/*** extra headers if needed ***/
#include <Modules/Input/Service/Context.h>
	/*** end header inclusion ***/

namespace Core { struct Intent; }

namespace Input
{
	class KeyBindings
	{
	public:
		void createContext(const std::string& contextName);
		Context& getContext(const std::string& contextName);
		void pushContext(const std::string& contextName, uint32_t priority);
		void popContext(const std::string contextName);

		bool findBinding(const Event& e, Core::Intent& out) const;

	private:
		std::unordered_map<std::string, Context> m_contexts;
		std::map<uint32_t, Context*> m_activeContexts;
	};
}
