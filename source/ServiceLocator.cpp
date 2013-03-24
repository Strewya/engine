//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "ServiceLocator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ServiceLocator::ServiceLocator()
		: _graphics(nullptr), _engine(nullptr)
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
}