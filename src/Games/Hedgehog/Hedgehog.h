#pragma once
/********************************************
*	class:	HedgehogGame
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;

	class HedgehogGame
	{
	public:
		GraphicsSystem m_graphics;
		ScriptingSystem m_scripter;

		Time m_logicTimer;
		Time m_renderTimer;
		double m_timeScale;
		Window* m_window;


		bool init(Window& window);
		bool tick();
		bool shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);
	};
}
