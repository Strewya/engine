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
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;

	class Actionmap
	{
	private:
		typedef std::unordered_map<std::string, std::unique_ptr<Action>> ActionCache;
		ActionCache _cache;

	public:
		Actionmap();
		
		bool Contains(const char* name);
		bool Contains(const std::string& name);

		void Clear();
		bool Remove(const char* name);
		bool Remove(const std::string& name);

		bool Insert(const char* name, std::unique_ptr<Action> action);
		bool Insert(const std::string& name, std::unique_ptr<Action> action);

		Action* Retrieve(const char* name);
		Action* Retrieve(const std::string& name);
	};
}