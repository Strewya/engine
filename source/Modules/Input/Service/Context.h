#pragma once
/********************************************
	class:	Input::Context
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
#include <deque>
#include <unordered_map>
	/*** extra headers if needed ***/
#include <Engine/Intent.h>
#include <Modules/Input/Service/Converter.h>
#include <Modules/Input/Service/Event.h>
	/*** end header inclusion ***/

namespace Input
{
	class Context
	{
	public:
		Context();

		bool mapEvent(const Event& e, Core::Intent& out) const;

		void addAction(uint32_t intentID, const Event& e);
		void addRange(uint32_t intentID, const Event& e);
		void addState(uint32_t intentID, const Event& e);
		
		void addRangeConverter(uint32_t intentID, const Converter& c);
		void addRangeConverter(uint32_t intentID, double minIn, double maxIn, double minOut, double maxOut);

	private:
		std::deque< std::pair< Event, Core::Intent > > m_bindings;
		std::unordered_map<uint32_t, Converter> m_converters;
	};
}
