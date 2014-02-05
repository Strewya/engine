#pragma once
/********************************************
*	class:	HedgehogGame
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Window;

	class HedgehogGame
	{
	public:
		bool init(Window& window);
		bool tick();
		bool shutdown();

	private:

	};
}
