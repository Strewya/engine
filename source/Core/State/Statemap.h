#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/State.h"
	/*** end header inclusion ***/

namespace Core
{
	/*
		Statemap class
		Description:
			Used to map States to a String identifier, as states themselves currently dont have names.
	*/
	class Statemap
	{
	private:

		typedef std::unordered_map<InstanceID, StateUptr> StateCache;
		StateCache _states;

	public:
		Statemap();
		Statemap(const Statemap& rhs);
		Statemap(Statemap&& rhs);
		Statemap& operator=(const Statemap& rhs);
		Statemap& operator=(Statemap&& rhs);

		bool contains(InstanceID id);

		void clear();
		bool destroy(InstanceID id);

		bool insert(StateUptr state);

		StateRptr retrieve(InstanceID id);
	};
}