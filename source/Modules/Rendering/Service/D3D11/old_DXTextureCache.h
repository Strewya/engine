#ifndef GRAPHICS_DXTEXTURECACHE_H_
#define GRAPHICS_DXTEXTURECACHE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <stack>
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXTexture.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXTextureCache
	{
	public:
		DXTextureCache();
		~DXTextureCache();

		bool Valid(uint index) const;
		
		uint Insert(const DXTexture& texture);
		int getHandle(const char* filename) const;
		int getHandle(const String& filename) const;
		const DXTexture& getTexture(uint index) const;

		void Destroy(uint index);
		void DestroyAll();

	private:
		struct DataWrapper
		{
			uint id;
			bool valid;
			DXTexture texture;
		};

		std::vector<DataWrapper> _cache;
		std::stack<uint> _freeSlots;
		
		DXTextureCache(const DXTextureCache& rhs);
		DXTextureCache& operator=(const DXTextureCache& rhs);
	};
}

#endif //GRAPHICS_TEXTURECACHE_H_