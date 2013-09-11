#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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
		SpriteInfo(const std::string& name, const Util::Rect& srcRect);
		
		const std::string& getName() const;
		const Util::Rect& getSrcRect() const;

		void setName(const char* name);
		void setName(const std::string& name);
		void setSrcRect(const Util::Rect& srcRect);
	
	private:
		std::string _name;
		Util::Rect _srcRect;
	};
}