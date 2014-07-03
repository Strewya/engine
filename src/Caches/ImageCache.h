#pragma once
/********************************************
*	class:	ImageLoader, ImageCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/ResourceCache.h>
#include <DataStructs/Image.h>
/******* end header inclusion *******/

namespace Core
{
	class FileLoader;
	class TextureCache;

	class ImageLoader
	{
	public:
		typedef ObjectContainer<Image> ImageData;
		typedef std::function<uint32_t(const char*)> Filter;

		ImageLoader() = default;
		ImageLoader(FileLoader& fileLoader, TextureCache& textures);

		bool load(ImageData& images, DataFile& filename, uint32_t fileID) const;
		bool reload(ImageData& images, DataFile& filename, uint32_t fileID) const;
		void unloadOne(ImageData& images, uint32_t imgID) const;
		void unloadFile(ImageData& images, uint32_t fileID) const;
		void unloadAll(ImageData& images) const;

	private:
		FileLoader* m_fileLoader;
		TextureCache* m_textures;

		bool processLoading(ImageData& images, DataFile& file, uint32_t fileID, const Filter& filter) const;
		bool parseImage(Image& img, DataFile& file, uint32_t fileID, const char* textureName, uint32_t defaultW, uint32_t defaultH) const;
	};

	class ImageCache : public ResourceCache < Image, ImageLoader > {};

}
