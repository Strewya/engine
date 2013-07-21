//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/TextureData.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	TextureData::TextureData()
		: width(0), height(0)
	{}

	TextureData::TextureData(uint32_t w, uint32_t h)
		: width(w), height(h)
	{}
}