#ifndef _SGMappedInput_h_
#define _SGMappedInput_h_
/********************************************
	class:	MappedInput
	usage:	
********************************************/
	/*** common header ***/
#include "Defines.h"
#include <map>
#include <set>
	/*** extra headers if needed ***/

	/*** end header inclusion ***/

namespace SG
{
	class MappedInput
	{
	private:
		std::set<String> _actions;
		std::set<String> _states;
		std::map<String, double> _ranges;
	public:
		void AddAction(const String action);
		void AddState(const String state);
		void AddRange(const String range, double value);

		// Consumption helpers
		void EatAction(const String& action);
		void EatState(const String& state);
		void EatRange(const String& range);
		
		bool HasAction(const String& action);
		bool HasState(const String& state);
		bool HasRange(const String& range);

		double RangeValue(const String& range);

		bool IsEmpty();

		void ClearActions();
		void ClearRanges();
	};
}

#endif //_SGMappedInput_h_