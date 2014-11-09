//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GuiSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool GuiSystem::init()
	{
		bool status = true;



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
		if(we.m_type == WE_MOUSEBUTTON && we.m_mouseButton.m_isDown)
		{
			for(auto& button : m_buttons)
			{
				if(button.activator == we.m_mouseButton.m_button)
				{
					button.onClick();
					return true;
				}
			}
		}
		return false;
	}

	void GuiSystem::draw(GraphicsSystem& graphics)
	{

	}

	void GuiSystem::canvas(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color)
	{

	}

	void GuiSystem::button(std::string name, std::string parent, Vec2 pos, Vec2 halfSize, Color color, Mouse::Keys activator, OnClickFunction onClick)
	{
		m_buttons.emplace_back(Button{name, pos, halfSize, color, onClick, activator});
	}

	void GuiSystem::label(std::string name, std::string parent, std::string text, Vec2 pos, Color color, uint32_t justification, bool italic)
	{

	}

	void GuiSystem::removeElement(std::string name)
	{

	}
}
