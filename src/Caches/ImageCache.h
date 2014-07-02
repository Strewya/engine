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
	class TextureCache;

	class ImageLoader
	{
	public:
		typedef std::vector<Image> ImageVector;
		typedef std::function<bool(const Image&)> Inserter;

		ImageLoader() = default;
		ImageLoader(TextureCache& textures);

		bool load(ImageVector& images, std::vector<uint32_t>* outIDs, DataFile& file) const;
		bool reload(ImageVector& images, std::vector<uint32_t>* outIDs, DataFile& file) const;
		bool unload(ImageVector& images, uint32_t id) const;
		bool unloadAll(ImageVector& images) const;

	private:
		TextureCache* m_textures;

		bool processLoading(ImageVector& images, DataFile& file, const Inserter& inserter) const;
		bool parseImage(Image& img, DataFile& file, float textureW, float textureH, uint32_t defaultW, uint32_t defaultH) const;
	};

	class ImageCache : public ResourceCache < Image, ImageLoader > {};

}
