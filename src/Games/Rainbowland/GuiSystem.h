#pragma once
/********************************************
*	class:	GuiSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <functional>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Input/KeyCodes.h>
#include <Util/Color.h>
#include <Util/Vec2.h>
#include <Window/WindowEvent.h>
/******* end header inclusion *******/

namespace Core
{
	class GraphicsSystem;
	
	class GuiSystem
	{
	public:
		typedef std::function<void()> OnClickFunction;
		bool init();
		bool shutdown();

		bool handleEvent(WindowEvent& we);
		void draw(GraphicsSystem& graphics);

		void canvas(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color);
		void button(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color, Mouse::Keys activator, OnClickFunction onClick);
		void label(std::string name, std::string parent, std::string text, Vec2 pos, Color color, uint32_t justification, bool italic);

		void removeElement(std::string name);

	private:
		struct Canvas
		{
			std::string name;
			Vec2 pos;
			Vec2 halfSize;
			Color color;
		};
		struct Button
		{
			std::string name;
			Vec2 pos;
			Vec2 halfSize;
			Color color;
			OnClickFunction onClick;
			Mouse::Keys activator;
		};
		struct Label
		{
			std::string name;
			std::string text;
			Vec2 pos;
			Color color;
			uint32_t justification;
			bool italic;
		};
		
		std::vector<Canvas> m_canvases;
		std::vector<Button> m_buttons;
		std::vector<Label> m_labels;
	};
}
