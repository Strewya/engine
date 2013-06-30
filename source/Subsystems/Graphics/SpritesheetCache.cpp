//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/SpritesheetCache.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Subsystems/Graphics/ITextureCache.h"
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



	Spritesheet& SpritesheetCache::getSpritesheet(uint32_t handle)
	{
		if(Exists(handle))
		{
			return _cache[handle];
		}
		throw std::exception("Attempted to get inexisting sheet!");
	}



	uint32_t SpritesheetCache::getSpritesheetHandle(const String& name)
	{
		return getSpritesheetHandle(name.c_str());
	}

	uint32_t SpritesheetCache::getSpritesheetHandle(const char* name)
	{
		auto it = _Find(name);
		if(it != _cache.end())
		{
			return it-_cache.begin();
		}
		return NOT_FOUND;
	}



	uint32_t SpritesheetCache::CreateEmpty(const char* sheetName)
	{
		auto it = _Find(sheetName);
		if(it == _cache.end())
		{
			_cache.emplace_back();
			_cache.back().setSpritesheetName(sheetName);
			return _cache.size()-1;
		}
		return it-_cache.begin();
	}

	uint32_t SpritesheetCache::CreateEmpty(const String& sheetName)
	{
		return CreateEmpty(sheetName.c_str());
	}



	uint32_t SpritesheetCache::LoadFromFile(const String& filename)
	{
		return LoadFromFile(filename.c_str());
	}

	uint32_t SpritesheetCache::LoadFromFile(const char* filename)
	{
		if(!_script->DoFile(filename))
			throw std::exception("Spritesheet definition has failed to parse, see the log file for reasons.");
		
		String name;
		_script->GetField("name", -1);
		_script->Pop(name);
		uint32_t sheetHandle = CreateEmpty(name);
		Spritesheet& sheet = getSpritesheet(sheetHandle);

		_script->GetField("texture", -1);
		_script->Pop(name);
		sheet.setTextureName(name);
		InstanceID texID = _textureCache->getTexture(name.c_str(), nullptr);
		sheet.setTexture(texID);

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
		return sheetHandle;
	}



	bool SpritesheetCache::Exists(uint32_t handle)
	{
		return handle < _cache.size();
	}
}