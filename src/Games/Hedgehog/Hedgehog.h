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
#include <Caches/AnimationCache.h>
#include <Caches/SpritesheetCache.h>
#include <Caches/TextureCache.h>
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
		
		
		//systems
		GraphicsSystem m_graphics;
		InputSystem m_input;
		AnimationSystem m_animation;
		//caches
		TextureCache m_textureCache;
		SpritesheetCache m_spritesheetCache;
		AnimationCache m_animationCache;

		Player m_player;
		
		Time m_logicTimer;
		Time m_renderTimer;
		Window* m_window;
		bool m_isRunning;

		std::vector<std::function<bool(const WindowEvent&)>> m_messageHandlers;

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);

	private:
		void onFileChanged(uint32_t index);
	};
}
