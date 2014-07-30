//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <DataStructs/Animation.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Caches/ImageCache.h>
#include <Util/DataFile.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool parseAnimation(Animation& outAnimation, DataFile& file, const AnimationDefaults& defaults, const ImageCache& images)
	{
		auto success = false;

		auto name = file.getString(-2, "");
		auto duration = Time::secondsToMicros(file.getFloat("duration", defaults.duration));
		auto loops = file.getBool("loop", defaults.loops);

		for( file.ipairs("images"); file.next(); )
		{
			auto imgName = file.getString(-1, "");
			auto imgID = images.getImageID(imgName.c_str());
			if( imgID != 0 )
			{
				outAnimation.m_sequence.emplace_back(imgID);
			}
		}

		if( !name.empty() && duration > 0 && !outAnimation.m_sequence.empty() )
		{
			//anim valid, parse images
			outAnimation.m_name = name;
			outAnimation.m_duration = duration;
			outAnimation.m_loops = loops;

			success = true;
		}
		else
		{
			DEBUG_INFO("Animation in ", file.getFilename(), " spritesheet is invalid: ", name, ",", duration, ",", outAnimation.m_sequence.size());
		}

		return success;
	}
}
