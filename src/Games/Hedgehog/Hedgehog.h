#pragma once
/********************************************
*	class:	HedgehogGame
*	usage:
********************************************/
/******* C++ headers *******/
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Animation/AnimationSystem.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Color.h>
#include <Util/Time.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;
	class WindowEvent;

	class Player
	{
	public:
		Transform m_transform;
		Color m_tint;
		AnimationData m_animationData;
		Time m_mainTimer;
	};

	class HedgehogGame
	{
	public:
		//this will never be exposed to lua!!
		ScriptingSystem m_scripter;
		//these can be exposed to lua
		GraphicsSystem m_graphics;
		InputSystem m_input;
		AnimationSystem m_animation;

		Player m_player;
		
		double m_logicTimeScale;
		Time m_logicTimer;
		Time m_renderTimer;
		Window* m_window;
		bool m_isRunning;

		std::vector<std::function<bool(const WindowEvent&)>> m_messageHandlers;

		bool init(Window& window);
		bool tick();
		bool shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);
	};
}
