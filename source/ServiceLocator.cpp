//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "ServiceLocator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ServiceLocator::ServiceLocator()
		: _graphics(nullptr), _engine(nullptr), _input(nullptr), _script(nullptr)
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