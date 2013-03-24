//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/SpritesheetCache.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "ResourceLocator.h"
#include "Subsystems/Graphics/TextureCache.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Dimensional.h"
	/*** end headers ***/

namespace Graphics
{
	SpritesheetCache::SpritesheetCache()
	{
	}

	void SpritesheetCache::setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services)
	{
		_textureCache = &resources.getTextureCache();
	}

	std::deque<Spritesheet>::iterator SpritesheetCache::_Find(const char* sheetName)
	{
		return std::find_if(_cache.begin(), _cache.end(),
			[&sheetName](const Spritesheet& sheet)
			{
				return sheet.getSpritesheetName().compare(sheetName) == 0;
		});
	}

	Spritesheet& SpritesheetCache::getSpritesheet(const char* sheetName)
	{
		auto it = _Find(sheetName);
		if(it != _cache.end())
		{
			return *it;
		}
		throw std::exception("Attempted to get inexisting sheet!");
	}

	Spritesheet& SpritesheetCache::getSpritesheet(const String& sheetName)
	{
		return getSpritesheet(sheetName.c_str());
	}

	Spritesheet& SpritesheetCache::CreateEmpty(const char* sheetName)
	{
		auto it = _Find(sheetName);
		if(it == _cache.end())
		{
			_cache.emplace_back();
			return _cache.back();
		}
		return *it;
	}

	Spritesheet& SpritesheetCache::CreateEmpty(const String& sheetName)
	{
		return CreateEmpty(sheetName.c_str());
	}

	Spritesheet& SpritesheetCache::LoadFromFile(const String& filename)
	{
		return LoadFromFile(filename.c_str());
	}

	Spritesheet& SpritesheetCache::LoadFromFile(const char* filename)
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
		sheet.setTexture(_textureCache->getTexture(name));

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

	bool SpritesheetCache::Exists(const char* sheetName)
	{
		return _Find(sheetName) != _cache.end();
	}

	bool SpritesheetCache::Exists(const String& sheetName)
	{
		return Exists(sheetName.c_str());
	}
}