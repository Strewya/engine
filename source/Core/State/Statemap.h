#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"

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

		typedef std::unordered_map<String, StateUptr> StateCache;
		StateCache _states;

	public:
		Statemap();
		Statemap(const Statemap& rhs);
		Statemap(Statemap&& rhs);
		Statemap& operator=(const Statemap& rhs);
		Statemap& operator=(Statemap&& rhs);

		bool contains(const char* name);
		bool contains(const String& name);

		void clear();
		bool destroy(const char* name);
		bool destroy(const String& name);

		bool insert(const char* name, StateUptr state);
		bool insert(const String& name, StateUptr state);

		StateRptr retrieve(const char* name);
		StateRptr retrieve(const String& name);
	};
}