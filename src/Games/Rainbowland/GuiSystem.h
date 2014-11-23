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
		void label(std::string name, std::string parent, uint32_t font, std::string text, Vec2 pos, Vec2 scale, Color color, uint32_t justification, bool italic);

		void editLabel(std::string name, std::string text);

		void removeElement(std::string name);

	private:
		struct GuiElement
		{
			virtual ~GuiElement() {};
			std::string name;
			Vec2 pos;
			GuiElement* parent;
			std::vector<GuiElement*> children;

			virtual bool handleEvent(const WindowEvent& we, Vec2 windowSize) { return false; };
			void draw(GraphicsSystem& graphics);
			virtual void drawSelf(GraphicsSystem& graphics) = 0;
		};
		
		struct Panel : public GuiElement
		{
			Vec2 halfSize;
			Color color;

			void drawSelf(GraphicsSystem& graphics);
		};

		struct Button : public GuiElement
		{
			Vec2 halfSize;
			Color color;
			OnClickFunction onClick;
			Mouse::Keys activator;

			bool handleEvent(const WindowEvent& we, Vec2 windowSize);
			void drawSelf(GraphicsSystem& graphics);
		};

		struct Label : public GuiElement
		{
			std::string text;
			Vec2 scale;
			Color color;
			uint32_t font;
			uint32_t justification;
			bool italic;

			void drawSelf(GraphicsSystem& graphics);
		};
		
		friend struct GuiSystem::Button;
		Vec2 m_windowSize;
		std::vector<std::unique_ptr<GuiElement>> m_elements;

		void removeElement(GuiElement* element);
	};
}
