#pragma once
/********************************************
*	class:	Stopwatch
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Stopwatch
	{
	public:
		Stopwatch();

		void updateBy(int64_t deltaMicros);
		void reset();
		void setTimeScale(double scale);
		
		double getTimeScale() const;
		int64_t getCurrentMicros() const;
		int32_t getDeltaMicros() const;
		float getDeltaTime() const;


	private:
		int64_t m_currentMicros;
		int64_t m_oldMicros;
		int64_t m_deltaMicros;
		double m_timeScale;
		float m_deltaTime;
	};
}
