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
		m_elements.emplace_back(std::make_unique<GuiElement>(new Panel{"root", {}, {}, windowSize / 2, {0, 0, 0, 0}}));
		
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
			if(element->handleEvent(we))
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

	void GuiSystem::draw(GraphicsSystem& graphics, GuiElement& element)
	{
		switch(element.type)
		{
			case GuiElement::PanelType:
				draw(graphics, m_panels[element.data], element.pos);
				break;
			case GuiElement::ButtonType:
				draw(graphics, m_buttons[element.data], element.pos);
				break;
			case GuiElement::LabelType:
				draw(graphics, m_labels[element.data], element.pos);
				break;
		}
		
	}

	void GuiSystem::draw(GraphicsSystem& graphics, Panel& panel, Vec2 pos)
	{
		Transform t;
		t.position = pos;
		graphics.drawQuad(t, panel.halfSize, panel.color);
	}
	
	void GuiSystem::draw(GraphicsSystem& graphics, Button& button, Vec2 pos)
	{
		Transform t;
		t.position = pos;
		graphics.drawQuadPolygon(t, {{}, button.halfSize.x, button.halfSize.y}, button.color);
	}

	void GuiSystem::draw(GraphicsSystem& graphics, Label& label, Vec2 pos)
	{
		Transform t;
		t.position = pos;
		graphics.drawText(label.font, label.text, t, label.color, label.justification, label.italic);
	}
	
	void GuiSystem::panel(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color)
	{
		auto uniqueName = filterFind(m_elements, [&](const GuiElement& ge){return ge.name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const GuiElement& ge){ return ge.name == parent; });
			if(parentIndex != m_elements.size())
			{
				m_elements[parentIndex].children.emplace_back(m_elements.size());
				m_elements.emplace_back(GuiElement{GuiElement::PanelType, m_panels.size(), parentIndex, name, pos + m_elements[parentIndex].pos});
				m_panels.emplace_back(Panel{m_elements.size() - 1, halfSize, color});
			}
		}
	}

	void GuiSystem::button(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color, Mouse::Keys activator, OnClickFunction onClick)
	{
		auto uniqueName = filterFind(m_elements, [&](const GuiElement& ge){return ge.name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const GuiElement& ge){ return ge.name == parent; });
			if(parentIndex != m_elements.size())
			{
				m_elements[parentIndex].children.emplace_back(m_elements.size());
				m_elements.emplace_back(GuiElement{GuiElement::ButtonType, m_buttons.size(), parentIndex, name, pos + m_elements[parentIndex].pos});
				m_buttons.emplace_back(Button{m_elements.size() - 1, halfSize, color, onClick, activator});
			}
		}
	}

	void GuiSystem::label(std::string name, std::string parent, uint32_t font, std::string text, Vec2 pos, Color color, uint32_t justification, bool italic)
	{
		auto uniqueName = filterFind(m_elements, [&](const GuiElement& ge){return ge.name == name; });
		if(uniqueName == m_elements.size())
		{
			auto parentIndex = filterFind(m_elements, [&](const GuiElement& ge){ return ge.name == parent; });
			if(parentIndex != m_elements.size())
			{
				m_elements[parentIndex].children.emplace_back(m_elements.size());
				m_elements.emplace_back(GuiElement{GuiElement::LabelType, m_labels.size(), parentIndex, name, pos + m_elements[parentIndex].pos});
				m_labels.emplace_back(Label{m_elements.size() - 1, text, color, font, justification, italic});
			}
		}
	}

	void GuiSystem::removeElement(std::string name)
	{
		if(name == "root") return;

		auto rIndex = filterFind(m_elements, [&](const GuiElement& ge){return name == ge.name; });
		if(rIndex != m_elements.size())
		{
			auto pIndex = m_elements[rIndex].parent;
			auto cIndex = valueFind(m_elements[pIndex].children, rIndex);
			m_elements[pIndex].children.erase(m_elements[pIndex].children.begin() + cIndex);
			std::vector<uint32_t> removeList{rIndex};
			for(uint32_t i = 0; i < removeList.size(); ++i)
			{
				removeList.insert(removeList.end(), m_elements[removeList[i]].children.begin(), m_elements[removeList[i]].children.end());
			}
			std::sort(removeList.begin(), removeList.end(), std::greater<uint32_t>());
			std::vector<GuiElement> removedElements;
			for(auto r : removeList)
			{
				removedElements.emplace_back(GuiElement{m_elements[r].type, m_elements[r].data});
			}
			
			for(auto elem : removeList)
			{
				removeElement(elem);
			}
		}
	}
}
