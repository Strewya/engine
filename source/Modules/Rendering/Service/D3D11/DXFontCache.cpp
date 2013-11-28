//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXFontCache.h"
	/*** C++ headers ***/
#include <stdexcept>
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXFontCache::DXFontCache()
	{
		_cache.reserve(16);
	}

	DXFontCache::~DXFontCache()
	{
		DestroyAll();
	}

	bool DXFontCache::Valid(uint32_t index) const
	{
		return (0 <= index && index < _cache.size() && _cache[index].valid);
	}
		
	int DXFontCache::getHandle(const char* name) const
	{
		for(auto& it : _cache)
		{
			if(it.font.info.getName().compare(name) == 0)
			{
				return it.id;
			}
		}
		return NOT_FOUND;
	}

	int DXFontCache::getHandle(const String& name) const
	{
		return getHandle(name.c_str());
	}
		
	uint32_t DXFontCache::Insert(const DXFont& font)
	{
		int index = getHandle(font.info.getName().c_str());
		if(index != NOT_FOUND)
		{
			return index;
		}
		if(_freeSlots.empty())
		{
			DataWrapper data;
			data.id = _cache.size();
			data.font = font;
			data.valid = true;
			_cache.push_back(data);
			return data.id;
		}
		else
		{
			index = _freeSlots.top();
			_freeSlots.pop();
			_cache[index].font = font;
			_cache[index].valid = true;
			return index;
		}
	}

	const DXFont& DXFontCache::getFont(uint32_t index) const
	{
		if(!Valid(index))
		{
			throw std::runtime_error("Attempted to dereference inexisting font");
		}
		return _cache[index].font;
	}

	void DXFontCache::Destroy(uint32_t index)
	{
		if(Valid(index))
		{
			_cache[index].font.Release();
			_cache[index].valid = false;
			_freeSlots.push(index);
		}
	}

	void DXFontCache::DestroyAll()
	{
		for(auto& it : _cache)
		{
			it.font.Release();
			it.valid = false;
		}
		while(!_freeSlots.empty())
		{
			_freeSlots.pop();
		}
	}
}
