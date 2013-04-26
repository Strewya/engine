#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <stack>
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXFont.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXFontCache
	{
	public:
		DXFontCache();
		~DXFontCache();

		bool Valid(uint index) const;
		
		uint Insert(const DXFont& font);
		int getHandle(const char* name) const;
		int getHandle(const String& name) const;
		const DXFont& getFont(uint index) const;

		void Destroy(uint index);
		void DestroyAll();

	private:
		struct DataWrapper
		{
			uint id;
			bool valid;
			DXFont font;
		};

		std::vector<DataWrapper> _cache;
		std::stack<uint> _freeSlots;
		
		DXFontCache(const DXFontCache& rhs);
		DXFontCache& operator=(const DXFontCache& rhs);
	};
}