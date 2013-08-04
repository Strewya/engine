//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/ServiceLocator.h"
	/*** C++ headers ***/
#include <cassert>
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ServiceLocator::ServiceLocator()
		: _engine(nullptr), _graphics(nullptr), _input(nullptr), _script(nullptr)
	{
	}

	void ServiceLocator::Register(Graphics::IRenderer* graphics)
	{
		_graphics = graphics;
	}
	
	Graphics::IRenderer& ServiceLocator::getGraphics() const
	{
		assert(_graphics != nullptr);
		return *_graphics;
	}

	void ServiceLocator::Register(Engine* engine)
	{
		_engine = engine;
	}
	
	Engine& ServiceLocator::getEngine() const
	{
		assert(_engine != nullptr);
		return *_engine;
	}

	void ServiceLocator::Register(Script::Engine* script)
	{
		_script = script;
	}

	Script::Engine& ServiceLocator::getScript() const
	{
		assert(_script != nullptr);
		return *_script;
	}

	void ServiceLocator::Register(Input::Engine* input)
	{
		_input = input;
	}

	Input::Engine& ServiceLocator::getInput() const
	{
		assert(_input != nullptr);
		return *_input;
	}
}
