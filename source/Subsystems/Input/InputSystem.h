#ifndef _SGInputSystem_h_
#define _SGInputSystem_h_
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
#include "SGInputContext.h"
#include "SGMappedInput.h"
	/*** end header inclusion ***/

namespace SG
{
	//fwd decs
	//end fwds

	class InputSystem
	{
	private:
		//typedefs
		typedef std::multimap<int, Action*> CallbackTable;
		typedef std::unordered_map<String, InputContext::Ptr> ContextMap;
		

		//member variables
		CallbackTable _callbacks;
		ContextMap _contexts;

		InputContext* _activeContext;
		MappedInput _currentInput;

		//member methods
		InputSystem(InputSystem& is);
		InputSystem& operator=(InputSystem& is);
		
		void _LoadContexts(const String& file);
		bool _MapButtonToState(uint button, String& outState);
		bool _MapButtonToAction(uint button, String& outAction);
		void _ClearButton(uint button);
		
	public:
		InputSystem();
		~InputSystem();

		void Clear();
		void SetButtonState(uint button, bool pressed, bool repeated);
		void SetAxisValue(uint axis, double value);
		
		bool PushContext(const String& name);
		bool PopContext();

		void Dispatch();
		
		void RegisterCallback(Action* callback, int priority);

	};
}

#endif //_SGInputSystem_h_