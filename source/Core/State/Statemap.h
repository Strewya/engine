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
#include "State.h"
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

		typedef std::unordered_map<String, std::unique_ptr<State>> StateCache;
		StateCache _states;

	public:
		Statemap();
		Statemap(const Statemap& rhs);
		Statemap(Statemap&& rhs);
		Statemap& operator=(const Statemap& rhs);
		Statemap& operator=(Statemap&& rhs);

		bool Contains(const char* name);
		bool Contains(const String& name);

		void Clear();
		bool Remove(const char* name);
		bool Remove(const String& name);

		//bool Insert(const char* name, State* state);
		//bool Insert(const String& name, State* state);
		bool Insert(const char* name, std::unique_ptr<State> state);
		bool Insert(const String& name, std::unique_ptr<State> state);

		State* Retrieve(const char* name);
		State* Retrieve(const String& name);
	};
}