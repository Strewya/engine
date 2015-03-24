#pragma once
/********************************************
*	class:	Time
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Clock
   {
   public:
      Clock();

      void update();
      void updateBy(uint64_t deltaMicros);

      uint32_t getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime);

      static uint64_t getRealTimeMicros();
      uint64_t getLastRealTimeMicros() const;

      uint64_t getCurrentMicros() const;
      uint32_t getDeltaMicros() const;
      float getDeltaSeconds() const;

      void setTimeScale(double timeScale);
      double getTimeScale() const;

   protected:
      uint64_t m_oldRealTime;
      double m_timeScale;
      uint64_t m_lastMicros;
      uint64_t m_currentMicros;
      uint64_t m_deltaMicros;
      float m_deltaSeconds;
   };

   class Timer
   {
   public:
      Timer();

      void updateBy(uint32_t deltaMicros);

      void setTimeScale(float timeScale);
      float getTimeScale() const;

      uint32_t getCurrentMicros() const;
      uint32_t getDeltaMicros() const;
      float getDeltaSeconds() const;

      void reset(uint32_t micros = 0);

   protected:
      uint32_t m_currentMicros;
      uint32_t m_deltaMicros;
      float m_deltaSeconds;
      float m_timeScale;
   };

   class CooldownTimer
   {
   public:
      CooldownTimer();
      CooldownTimer(uint32_t durationMicros);

      void updateBy(uint32_t deltaMicros);
      bool hasElapsed() const;
      uint32_t getRemainingMicros() const;
      float getPercentDone() const;
      void reset();

      uint32_t getDurationMicros() const;
      void setDurationMicros(uint32_t durationMicros);
      const Timer& getTimer() const;

   protected:
      Timer m_timer;
      uint32_t m_durationMicros;
   };

   class PeriodicTimer
   {
   public:
      PeriodicTimer();
      PeriodicTimer(uint32_t periodMicros);

      void updateBy(uint32_t deltaMicros);
      bool hasElapsed() const;
      uint32_t getRemainingMicros() const;
      float getPercentDone() const;
      void period();
      void hardReset();

      uint32_t getPeriodMicros() const;
      void setPeriodMicros(uint32_t periodMicros);
      const Timer& getTimer() const;

   protected:
      Timer m_timer;
      uint32_t m_periodMicros;
   };

   class DebugTimingOutputString
   {
   public:
      DebugTimingOutputString(std::string format = "%t");
      ~DebugTimingOutputString();
   private:
      uint64_t m_startTime;
      std::string m_format;
   };

   uint32_t secondsToMicros(uint32_t s);
   uint32_t secondsToMicros(float s);
   uint32_t secondsToMilis(uint32_t s);
   uint32_t secondsToMilis(float s);
   uint32_t milisToMicros(uint32_t ms);
   float milisToSeconds(uint32_t ms);
   float microsToSeconds(uint32_t us);
}
