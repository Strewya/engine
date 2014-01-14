#pragma once
/********************************************
*	class:	KeybindSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Window/WindowEvent.h>
/******* end header inclusion *******/

namespace Core
{
	typedef std::vector<WindowEvent> EventVector_t;
	typedef std::vector<uint32_t> CommandVector_t;

	class KeybindSystem
	{
	public:
		bool init();
		void shutdown();

		void update(const EventVector_t& inputEvents);

	private:
		CommandVector_t m_commands;
	};
}