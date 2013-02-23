//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "ServiceLocator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	ServiceLocator::ServiceLocator()
		: _graphics(nullptr)
	{
	}

	void ServiceLocator::Register(Graphics::Interface* graphics)
	{
		_graphics = graphics;
	}
	
	Graphics::Interface& ServiceLocator::getGraphics() const
	{
		assert(_graphics != nullptr);
		return *_graphics;
	}
}