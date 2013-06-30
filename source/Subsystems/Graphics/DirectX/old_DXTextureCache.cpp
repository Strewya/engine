//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXTextureCache.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXTextureCache::DXTextureCache()
	{
		_cache.reserve(16);
	}

	DXTextureCache::~DXTextureCache()
	{
		DestroyAll();
	}

	bool DXTextureCache::Valid(uint index) const
	{
		return (0 <= index && index < _cache.size() && _cache[index].valid);
	}
		
	int DXTextureCache::getHandle(const char* filename) const
	{
		for(auto it = _cache.begin(); it != _cache.end(); ++it)
		{
			if(it->texture.info.getFilename().compare(filename) == 0)
			{
				return it->id;
			}
		}
		return NOT_FOUND;
	}

	int DXTextureCache::getHandle(const String& filename) const
	{
		return getHandle(filename.c_str());
	}
		
	uint DXTextureCache::Insert(const DXTexture& texture)
	{
		int index = getHandle(texture.info.getFilename().c_str());
		if(index != NOT_FOUND)
		{
			return index;
		}
		if(_freeSlots.empty())
		{
			DataWrapper data;
			data.id = _cache.size();
			data.texture = texture;
			data.valid = true;
			_cache.push_back(data);
			return data.id;
		}
		else
		{
			index = _freeSlots.top();
			_freeSlots.pop();
			_cache[index].texture = texture;
			_cache[index].valid = true;
			return index;
		}
	}

	const DXTexture& DXTextureCache::getTexture(uint index) const
	{
		if(Valid(index))
		{
			return _cache[index].texture;
		}
		throw std::exception("Attempted to dereference inexisting texture");
	}

	void DXTextureCache::Destroy(uint index)
	{
		if(Valid(index))
		{
			_cache[index].texture.Release();
			_cache[index].valid = false;
			_freeSlots.push(index);
		}
	}

	void DXTextureCache::DestroyAll()
	{
		for(auto it = _cache.begin(); it != _cache.end(); ++it)
		{
			it->texture.Release();
			it->valid = false;
		}
		while(!_freeSlots.empty())
		{
			_freeSlots.pop();
		}
	}
}