//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/SpritesheetCache.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Subsystems/Graphics/TextureCache.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Dimensional.h"
	/*** end headers ***/

namespace Graphics
{
	SpritesheetCache::SpritesheetCache()
		: _textureCache(nullptr), _script(nullptr)
	{
	}

	void SpritesheetCache::setReferences(const Core::ResourceLocator& resources, const Core::ServiceLocator& services)
	{
		_textureCache = &resources.getTextureCache();
		_script = &services.getScript();
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
			_cache.back().setSpritesheetName(sheetName);
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
		if(!_script->DoFile(filename))
			throw std::exception("Spritesheet definition has failed to parse, see the log file for reasons.");
		
		String name;
		_script->GetField("name", -1);
		_script->Pop(name);
		Spritesheet& sheet = CreateEmpty(name);

		_script->GetField("texture", -1);
		_script->Pop(name);
		sheet.setTextureName(name);
		sheet.setTexture(_textureCache->getTextureHandle(name));

		int frameCount;

		if(_script->GetField("frames", -1) && _script->GetObjLength(frameCount))
		{
			for(int i=1; i<=frameCount; ++i)
			{
				Util::Rect rect;
				_script->Push(i);
				_script->GetField(-2);
			
				_script->GetField("name", -1);
				_script->Pop(name);
				_script->GetField("rect", -1);
				_script->Pop(rect);
				_script->Pop();

				SpriteInfo si;
				si.setName(name);
				si.setSrcRect(rect);
				sheet.Insert(si);
			}
			_script->Pop();
		
			int animCount;
			if(_script->GetField("animations", -1) && _script->GetObjLength(animCount))
			{
				for(int anim=1; anim<=animCount; ++anim)
				{
					AnimationInfo animation;

					_script->Push(anim);
					_script->GetField(-2);
			
					_script->GetField("name", -1);
					_script->Pop(name);
					animation.setName(name);

					_script->GetField("sequence", -1);
					_script->GetObjLength(frameCount);
					for(int frame=1; frame<=frameCount; ++frame)
					{
						_script->Push(frame);
						_script->GetField(-2);
						_script->Pop(name);

						animation.AddToSequence(sheet.getSpriteHandle(name));
					}
					_script->Pop();

					_script->Pop();

					sheet.Insert(animation);
				}
				_script->Pop();
			}
		}

		_script->Pop();
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