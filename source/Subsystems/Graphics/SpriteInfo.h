#ifndef GRAPHICS_SPRITEINFO_H_
#define GRAPHICS_SPRITEINFO_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class SpriteInfo
	{
	public:
		SpriteInfo();
		SpriteInfo(const char* name, const Util::Rect& srcRect);
		SpriteInfo(const String& name, const Util::Rect& srcRect);
		
		const String& getName() const;
		const Util::Rect& getSrcRect() const;

		void setName(const char* name);
		void setName(const String& name);
		void setSrcRect(const Util::Rect& srcRect);
	
	private:
		String _name;
		Util::Rect _srcRect;
	};
}

#endif //GRAPHICS_SPRITEINFO_H_