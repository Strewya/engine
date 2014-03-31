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
	class ConfigFile;

	class ImageCache
	{
	public:
		bool init();
		bool shutdown();

		uint32_t getImageID(const char* name) const;
		const Image& getImage(uint32_t id) const;
		
		bool addImage(const Image& image, uint32_t* outIndex = nullptr);
		bool loadImage(ConfigFile& file, bool reload, uint32_t* outIndex = nullptr);

	private:
		std::vector<Image> m_images;

	};
}
