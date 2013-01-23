//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/SpritesheetCache.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Dimensional.h"
	/*** end headers ***/

namespace Graphics
{
	SpritesheetCache::SpritesheetCache()
	{
		_cache.reserve(16);
	}

	std::vector<SpritesheetCache::DataWrapper>::iterator SpritesheetCache::Find(const char* sheetName)
	{
		return std::find_if(_cache.begin(), _cache.end(),
			[&sheetName](const DataWrapper& wrapper)
			{
				return wrapper.valid && wrapper.sheet.getSpritesheetName().compare(sheetName) == 0;
		});
	}

	Spritesheet& SpritesheetCache::getSpritesheet(const char* sheetName)
	{
		auto it = Find(sheetName);
		if(it != _cache.end())
		{
			return it->sheet;
		}
		throw std::exception("Attempted to get inexisting sheet!");
	}

	Spritesheet& SpritesheetCache::getSpritesheet(const String& sheetName)
	{
		return getSpritesheet(sheetName.c_str());
	}

	Spritesheet& SpritesheetCache::CreateEmpty(const char* sheetName)
	{
		return Insert(Spritesheet(sheetName));
	}

	Spritesheet& SpritesheetCache::CreateEmpty(const String& sheetName)
	{
		return CreateEmpty(sheetName.c_str());
	}

	Spritesheet& SpritesheetCache::CreateFromFile(const String& filename)
	{
		return CreateFromFile(filename.c_str());
	}

	Spritesheet& SpritesheetCache::CreateFromFile(const char* filename)
	{
		Script::LuaEngine script;
		if(!script.DoFile(filename))
			throw std::exception("Spritesheet definition has failed to parse, see the log file for reasons.");
		
		String name;
		script.GetField("name", -1);
		script.Pop(name);
		Spritesheet& sheet = CreateEmpty(name);

		script.GetField("texture", -1);
		script.Pop(name);
		sheet.setTextureName(name);

		int frameCount;
		script.GetField("framecount", -1);
		script.Pop(frameCount);
		
		script.GetField("frames", -1);
		for(int i=1; i<=frameCount; ++i)
		{
			Util::Rect rect;
			script.Push(i);
			script.GetField(-2);
			
			script.GetField("name", -1);
			script.Pop(name);
			script.GetField("rect", -1);
			script.Pop(rect);
			script.Pop();

			SpriteInfo si;
			si.setName(name);
			si.setSrcRect(rect);
			sheet.Insert(si);
		}
		return sheet;
	}

	Spritesheet& SpritesheetCache::Insert(const Spritesheet& sheet)
	{
		auto it = Find(sheet.getSpritesheetName().c_str());
		if(it == _cache.end())
		{
			uint index;
			if(_freeSlots.empty())
			{
				DataWrapper data;
				index = data.id = _cache.size();
				_cache.push_back(data);
			}
			else
			{
				index = _freeSlots.top();
				_freeSlots.pop();
			}
			_cache[index].sheet = sheet;
			_cache[index].valid = true;
			return _cache[index].sheet;
		}
		return it->sheet;
	}

	bool SpritesheetCache::Exists(const char* sheetName)
	{
		return Find(sheetName) != _cache.end();
	}

	bool SpritesheetCache::Exists(const String& sheetName)
	{
		return Exists(sheetName.c_str());
	}

	void SpritesheetCache::DestroyAll()
	{
		_cache.clear();
		while(!_freeSlots.empty())
			_freeSlots.pop();
	}

	void SpritesheetCache::Destroy(const char* sheetName)
	{
		auto it = Find(sheetName);
		if(it != _cache.end())
		{
			it->valid = false;
			it->sheet.~Spritesheet();
			_freeSlots.push(it->id);
		}
	}

	void SpritesheetCache::Destroy(const String& sheetName)
	{
		Destroy(sheetName.c_str());
	}
}