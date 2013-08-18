#ifndef _SGInputInputContext_h_
#define _SGInputInputContext_h_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace SG
{
	class InputContext
	{
	public:
		//uint = Keys/Axis, String = Action/State/Range
		typedef std::map< uint, String > Mapping;
		typedef std::unique_ptr<InputContext> Ptr;

		InputContext(ticpp::Element* xml);

		bool MapButtonToState(uint e, String& out) const;
		bool MapButtonToAction(uint e, String& out) const;
		bool MapAxisToRange(uint e, String& out) const;

		String getName() const;
		bool isPropagate() const;
		void setPropagation(bool propagate);
		
		//CoR data
		InputContext* Push(InputContext* next);
		InputContext* Pop();

	private:
		String _name;
		bool _propagates;
		InputContext* _next;

		Mapping _states;
		Mapping _actions;
		Mapping _ranges;

		//Converter
	};


}

#endif //_SGInputInputContext_h_