//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Input/KeybindSystem.h>
/******* C++ headers *******/
#include <iostream>
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	bool KeybindSystem::init()
	{
		std::cout << "KeybindSystem init start" << std::endl;
		bool status = true;


		std::cout << "KeybindSystem init " << (status ? "OK" : "FAIL") << std::endl;
		return status;
	}

	void KeybindSystem::shutdown()
	{

	}

	void KeybindSystem::update(const EventVector_t& events)
	{
		
	}
}
