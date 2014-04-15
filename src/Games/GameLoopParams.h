#pragma once
/********************************************
*	class:	
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end header inclusion *******/

#define CORE_VARIABLE_STEP 0
#define CORE_CLAMPED_STEP 1
#define CORE_FIXED_STEP 2


#define CORE_FIXED_STEP_FPS 30ULL
#define CORE_CLAMPED_STEP_MIN_FPS 5ULL

#define CORE_MICROS_PER_FRAME     1000000ULL/CORE_FIXED_STEP_FPS
#define CORE_MAX_MICROS_PER_FRAME 1000000ULL/CORE_CLAMPED_STEP_MIN_FPS


#define CORE_STEP CORE_CLAMPED_STEP

namespace Core
{
	inline uint32_t getLogicUpdateCount(Time& timer, const uint64_t& microsPerFrame, float& outFraction, uint64_t& outUnusedMicros, uint64_t& outDroppedTime)
	{
		static const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;

		const uint32_t maxUpdateCount = static_cast<uint32_t>(maxUpdateTime / microsPerFrame);
		const uint32_t updateCount = timer.getFixedStepUpdateCount(microsPerFrame, outFraction, outUnusedMicros);
		outDroppedTime = updateCount > maxUpdateCount ? updateCount - maxUpdateCount : 0;
		outDroppedTime *= microsPerFrame;
		return updateCount <= maxUpdateCount ? updateCount : maxUpdateCount;
	}
}