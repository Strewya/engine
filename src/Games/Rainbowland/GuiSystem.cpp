//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GuiSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Util/CollisionChecks.h>
#include <Util/Rect.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool GuiSystem::init(Vec2 windowSize)
	{
		bool status = true;

		m_windowSize = windowSize;
		std::unique_ptr<Panel> ptr{new Panel{}};
		ptr->name = "root";
		ptr->halfSize = windowSize / 2;
		ptr->color.set(0, 0, 0, 0);
		m_elements.emplace_back(std::move(ptr));
		
		DEBUG_INIT(GuiSystem);
		return status;
	}

	bool GuiSystem::shutdown()
	{
		bool status = true;

		DEBUG_SHUTDOWN(GuiSystem);
		return status;
	}

	bool GuiSystem::handleEvent(WindowEvent& we)
	{
		for(auto& element : m_elements)
		{
			if(element->handleEvent(we, m_windowSize))
			{
				return true;
			}
		}
		return false;
	}

	void GuiSystem::draw(GraphicsSystem& graphics)
	{
		m_elements[0]->draw(graphics);
	}

	void GuiSystem::GuiElement::draw(GraphicsSystem& graphics)
	{
		drawSelf(graphics);
		for(auto* child : children)
		{
			child->draw(graphics);
		}
	}

	void GuiSystem::Panel::drawSelf(GraphicsSystem& graphics)
	{
		Transform t;
		t.position = pos;
		drawSolidQuad(graphics, halfSize, t, color);
	}
	
	bool GuiSystem::Button::handleEvent(const WindowEvent& we, Vec2 windowSize)
	{
		if(we.m_type == WE_MOUSEBUTTON &&
		   we.m_mouseButton.m_isDown &&
		   we.m_mouseButton.m_button == activator)
		{
			Vec2 clickedPoint{(float)we.m_mouseButton.m_x, (float)we.m_mouseButton.m_y};
			clickedPoint.x -= windowSize.x*0.5f;
			clickedPoint.y -= windowSize.y*0.5f;
			clickedPoint.y = -clickedPoint.y;
			Rect buttonRect{pos, halfSize.x, halfSize.y};
			if(isPointInsideRect(clickedPoint, buttonRect))
			{
				onClick();
				return true;
			}
		}
		return false;
	}

	void GuiSystem::Button::drawSelf(GraphicsSystem& graphics)
	{
		Transform t;
		t.position = pos;
		drawHollowQuad(graphics, halfSize, t, color);
	}

	void GuiSystem::Label::drawSelf(GraphicsSystem& graphics)
	{
		Transform t;
		t.position = pos;
		t.scale = scale;
		drawText(graphics, font, text, t, color, justification, italic);
	}
	
	void GuiSystem::panel(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color)
	{
		auto uniqueName = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){return ge->name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){ return ge->name == parent; });
			if(parentIndex != m_elements.size())
			{
				std::unique_ptr<Panel> ptr{new Panel{}};
				ptr->name = name;
				ptr->pos = pos + m_elements[parentIndex]->pos;
				ptr->parent = m_elements[parentIndex].get();
				ptr->halfSize = halfSize;
				ptr->color = color;
				m_elements[parentIndex]->children.emplace_back(ptr.get());
				m_elements.emplace_back(std::move(ptr));
			}
		}
	}

	void GuiSystem::button(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color, Mouse::Keys activator, OnClickFunction onClick)
	{
		auto uniqueName = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){return ge->name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){ return ge->name == parent; });
			if(parentIndex != m_elements.size())
			{
				std::unique_ptr<Button> ptr{new Button{}};
				ptr->name = name;
				ptr->pos = pos + m_elements[parentIndex]->pos;
				ptr->parent = m_elements[parentIndex].get();
				ptr->halfSize = halfSize;
				ptr->color = color;
				ptr->activator = activator;
				ptr->onClick = onClick;
				m_elements[parentIndex]->children.emplace_back(ptr.get());
				m_elements.emplace_back(std::move(ptr));
			}
		}
	}

	void GuiSystem::label(std::string name, std::string parent, uint32_t font, std::string text, Vec2 pos, Vec2 scale, Color color, TextJustification justification, bool italic)
	{
		auto uniqueName = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){return ge->name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){ return ge->name == parent; });
			if(parentIndex != m_elements.size())
			{
				std::unique_ptr<Label> ptr{new Label{}};
				ptr->name = name;
				ptr->pos = pos + m_elements[parentIndex]->pos;
				ptr->parent = m_elements[parentIndex].get();
				ptr->text = text;
				ptr->color = color;
				ptr->scale = scale;
				ptr->font = font;
				ptr->justification = justification;
				ptr->italic = italic;
				m_elements[parentIndex]->children.emplace_back(ptr.get());
				m_elements.emplace_back(std::move(ptr));
			}
		}
	}

	void GuiSystem::editLabel(std::string name, std::string text)
	{
		auto uniqueName = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge) { return ge->name == name; });
		if(uniqueName != m_elements.size())
		{
			Label& label = static_cast<Label&>(*m_elements[uniqueName]);
			label.text = text;
		}
	}

	void GuiSystem::removeElement(std::string name)
	{
		if(name == "root") return;

		auto rIndex = filterFind(m_elements, [&](const std::unique_ptr<GuiElement>& ge){return name == ge->name; });
		if(rIndex != m_elements.size())
		{
			auto* element = m_elements[rIndex].get();
			auto& parent = *element->parent;
			auto childIndex = valueFind(parent.children, element);
			parent.children.erase(parent.children.begin() + childIndex);

			removeElement(element);
		}
	}

	void GuiSystem::removeElement(GuiElement* ge)
	{
		for(auto* child : ge->children)
		{
			removeElement(child);
		}
		auto index = filterFind(m_elements, [=](const std::unique_ptr<GuiElement>& elem){return elem.get() == ge; });
		m_elements[index].swap(m_elements.back());
		m_elements.pop_back();
	}
}
