//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/SpritesheetCache.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Dimensional.h"
	/*** end headers ***/

namespace Graphics
{
	SpritesheetCache::~SpritesheetCache()
	{
		Clear();
	}

	void SpritesheetCache::setTextureCache(ITextureCache& cache)
	{
		_textureCache = &cache;
	}

	InstanceID SpritesheetCache::LoadSpritesheet(const char* name, Spritesheet** outPtr)
	{
		InstanceID id = NOT_FOUND;
		auto* asset = Acquire(name);
		if(asset != nullptr)
		{
			id = asset->id;
			if(outPtr != nullptr)
			{
				*outPtr = asset->dataPtr.get();
			}
		}
		return id;
	}

	Spritesheet* SpritesheetCache::getSpritesheet(uint32_t handle)
	{
		auto* asset = CheckLoaded(handle);
		return (asset != nullptr ? asset->dataPtr.get() : nullptr);
	}
		
	InstanceID SpritesheetCache::getSpritesheet(const char* name, Spritesheet** outPtr)
	{
		InstanceID id = NOT_FOUND;
		auto* asset = CheckLoaded(name);
		if(asset != nullptr)
		{
			id = asset->id;
			if(outPtr != nullptr)
			{
				*outPtr = asset->dataPtr.get();
			}
		}
		return id;
	}

	bool SpritesheetCache::Load(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset)
	{
		Script::Engine script;
		if(!script.DoFile(filename))
		{
			return false;
		}
		
		String name;
		script.GetField("name", -1);
		script.Pop(name);
		asset.reset(new Spritesheet(name));
		Spritesheet& sheet = *asset;

		script.GetField("texture", -1);
		script.Pop(name);
		sheet.setTextureName(name);
		InstanceID texID = _textureCache->getTexture(name.c_str(), nullptr);
		sheet.setTexture(texID);

		int frameCount;

		if(script.GetField("frames", -1) && script.GetObjLength(frameCount))
		{
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
			script.Pop();
		
			int animCount;
			if(script.GetField("animations", -1) && script.GetObjLength(animCount))
			{
				for(int anim=1; anim<=animCount; ++anim)
				{
					AnimationInfo animation;

					script.Push(anim);
					script.GetField(-2);
			
					script.GetField("name", -1);
					script.Pop(name);
					animation.setName(name);

					script.GetField("sequence", -1);
					script.GetObjLength(frameCount);
					for(int frame=1; frame<=frameCount; ++frame)
					{
						script.Push(frame);
						script.GetField(-2);
						script.Pop(name);

						animation.AddToSequence(sheet.getSpriteHandle(name));
					}
					script.Pop();

					script.Pop();

					sheet.Insert(animation);
				}
				script.Pop();
			}
		}

		script.Pop();
		return true;
	}
}