//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Animation.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Scripting/LuaStack.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	LoadResult loadAnimation(Animation& outAnimation, LuaStack& lua, size_t fileHash, const AnimationDefaults& defaults, ImageCache& images)
	{
		if( !lua.is<std::string>(-2) || !lua.is<luaTable>(-1) )
		{
			return {LoadResultFlag::Fail, "Invalid animation format"};
		}

		outAnimation.m_name = lua.to<std::string>(-2);
		outAnimation.m_duration = secondsToMicros(get(lua, "duration", defaults.duration));
		outAnimation.m_loops = get(lua, "loop", defaults.loops);

		for( lua.ipairs("images"); lua.next(); lua.pop(1) )
		{
			auto imgName = lua.to<std::string>(-1);
			auto imgID = images.getResourceID(imgName.c_str());
			if( imgID != INVALID_ID )
			{
				outAnimation.m_sequence.emplace_back(imgID);
			}
		}

		return {LoadResultFlag::Success};
	}

	void unloadAnimation(Animation& animation)
	{
		animation.m_duration = 0;
		animation.m_loops = false;
		animation.m_name.clear();
		animation.m_sequence.clear();
	}
}
