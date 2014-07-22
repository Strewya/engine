#pragma once
/********************************************
*	class:	ImageCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Image.h>
/******* end header inclusion *******/

namespace Core
{
	class ImageCache
	{
	public:
		bool init();
		bool shutdown();

		bool addImage(const Image& image, bool reload, uint32_t* outIndex = nullptr);
		
		uint32_t getImageID(const char* name) const;
		const Image& getImage(uint32_t id) const;

	private:
		std::vector<Image> m_images;
	};
}
