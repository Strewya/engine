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
#include <Util/ObjectPool.h>
/******* end header inclusion *******/

namespace Core
{
	class ResourceFile;
	class ScriptingSystem;
	class TextureCache;

	class ImageCache
	{
	public:
		bool init(ScriptingSystem& scripting, TextureCache& textures);
		bool shutdown();

		uint32_t getImageID(const char* name) const;
		const Image& getImage(uint32_t id) const;

		bool load(const ResourceFile& file);
		bool reload(const ResourceFile& file);
		bool unload(const ResourceFile& file);

	private:
		ScriptingSystem* m_scripting;
		TextureCache* m_textures;

		ObjectPool<Image> m_images;
		
	};
}
