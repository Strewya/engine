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
#include <Caches/FileHandlerCache.h>
#include <Caches/ImageCache.h>
#include <Caches/PackageCache.h>
#include <Caches/ScriptCache.h>
#include <Caches/SpritesheetCache.h>
#include <Caches/TextureCache.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Loaders/PackageLoader.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Color.h>
#include <Util/Time.h>
#include <Util/Transform.h>
#include <Util/Rect.h>
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
		Rect m_collisionRect;
		uint32_t m_animationPlayerID;
		uint32_t m_imageID;
	};

	class Prop
	{
	public:
		Transform m_transform;
		Color m_tint;
		Rect m_collisionRect;
		uint32_t m_imageID;
	};

	class HedgehogGame
	{
	public:
		//this will never be exposed to lua!! (lua calling itself through the scripting system really makes no sense)
		ScriptingSystem m_scripter;
		Time m_framerateTimer;
		
		//systems
		AnimationSystem m_animation;
		GraphicsSystem m_graphics;
		InputSystem m_input;
		//caches
		PackageCache m_packageCache;
		FileHandlerCache m_loadHandlerCache;
		FileHandlerCache m_reloadHandlerCache;
		FileHandlerCache m_unloadHandlerCache;
		AnimationCache m_animationCache;
		ImageCache m_imageCache;
		ScriptCache m_scriptCache;
		SpritesheetCache m_spritesheetCache;
		TextureCache m_textureCache;
		//loaders
		PackageLoader m_packageLoader;

		Camera m_camera;
		
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
	};
}
