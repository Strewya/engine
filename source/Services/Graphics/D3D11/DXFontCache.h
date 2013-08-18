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

		bool Valid(uint32_t index) const;
		
		uint32_t Insert(const DXFont& font);
		int getHandle(const char* name) const;
		int getHandle(const String& name) const;
		const DXFont& getFont(uint32_t index) const;

		void Destroy(uint32_t index);
		void DestroyAll();

	private:
		struct DataWrapper
		{
			uint32_t id;
			bool valid;
			DXFont font;
		};

		std::vector<DataWrapper> _cache;
		std::stack<uint32_t> _freeSlots;
		
		DXFontCache(const DXFontCache& rhs);
		DXFontCache& operator=(const DXFontCache& rhs);
	};
}