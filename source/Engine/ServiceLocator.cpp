//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Engine/ServiceLocator.h>
	/*** C++ headers ***/
#include <cassert>
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ServiceLocator::ServiceLocator()
		: m_engine(nullptr), m_graphics(nullptr), m_input(nullptr), m_script(nullptr)
	{
	}

	void ServiceLocator::bind(Graphics::IRenderer& graphics)
	{
		m_graphics = &graphics;
	}
	
	Graphics::IRenderer& ServiceLocator::getGraphics() const
	{
		assert(m_graphics != nullptr);
		return *m_graphics;
	}

	void ServiceLocator::bind(Engine& engine)
	{
		m_engine = &engine;
	}
	
	Engine& ServiceLocator::getEngine() const
	{
		assert(m_engine != nullptr);
		return *m_engine;
	}

	void ServiceLocator::bind(Script::Engine& script)
	{
		m_script = &script;
	}

	Script::Engine& ServiceLocator::getScript() const
	{
		assert(m_script != nullptr);
		return *m_script;
	}

	void ServiceLocator::bind(Input::Engine& input)
	{
		m_input = &input;
	}

	Input::Engine& ServiceLocator::getInput() const
	{
		assert(m_input != nullptr);
		return *m_input;
	}
}
