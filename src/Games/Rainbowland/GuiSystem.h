#pragma once
/********************************************
*	class:	GuiSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <functional>
#include <memory>
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
		bool init(Vec2 windowSize);
		bool shutdown();

		bool handleEvent(WindowEvent& we);
		void draw(GraphicsSystem& graphics);

		void panel(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color);
		void button(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color, Mouse::Keys activator, OnClickFunction onClick);
		void label(std::string name, std::string parent, uint32_t font, std::string text, Vec2 pos, Color color, uint32_t justification, bool italic);

		void removeElement(std::string name);

	private:
		struct GuiElement
		{
			std::string name;
			Vec2 pos;
			GuiElement* parent;
			std::vector<GuiElement*> children;

			virtual bool handleEvent(const WindowEvent& we) { return false; };
			void draw(GraphicsSystem& graphics) { drawSelf(graphics); for(auto* child : children) child-> };
			virtual void drawSelf(GraphicsSystem& graphics) = 0;
		};
		
		struct Panel : public GuiElement
		{
			Vec2 halfSize;
			Color color;

			void draw(GraphicsSystem& graphics);
		};

		struct Button : public GuiElement
		{
			Vec2 halfSize;
			Color color;
			OnClickFunction onClick;
			Mouse::Keys activator;

			bool handleEvent(const WindowEvent& we);
			void draw(GraphicsSystem& graphics);
		};

		struct Label : public GuiElement
		{
			std::string text;
			Color color;
			uint32_t font;
			uint32_t justification;
			bool italic;

			void draw(GraphicsSystem& graphics);
		};
		
		Vec2 m_windowSize;
		std::vector<std::unique_ptr<GuiElement>> m_elements;
	};
}
