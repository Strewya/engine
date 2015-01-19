#pragma once
/********************************************
*	class:	Time
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Time
   {
   public:
      static const double STOP_TIME;
      static const double NORMAL_TIME;

      static int64_t secondsToMicros(float sec);
      static int64_t milisToMicros(int32_t mili);
      static float milisToSeconds(int32_t mili);
      static float microsToSeconds(int64_t micros);
      static int32_t secondsToMilis(float sec);
      static int32_t microsToMilis(int64_t micros);

      static int32_t countMilisInMicros(int64_t micros);
      static int32_t countSecondsInMicros(int64_t micros);
      static int32_t countMinutesInMicros(int64_t micros);
      static int32_t countHoursInMicros(int64_t micros);

      static int32_t microsDelta(int64_t start, int64_t end);

      Time();

      void update();
      void updateBy(int64_t delta);
      uint32_t getFixedStepUpdateCount(uint64_t frameTime, float& ratio, uint64_t& remainderTime);

      uint64_t getLastRealTimeMicros() const;
      static uint64_t getRealTimeMicros();

      uint64_t getCurrentMicros() const;
      int32_t getDeltaMicros() const;
      float getDeltaTime() const;

      //use only for countup related timers
      void reset();

      void setTimeScale(double timeScale);
      double getTimeScale() const;

   protected:
      uint64_t m_oldRealTime;

      uint64_t m_lastMicros;
      uint64_t m_curMicros;
      int64_t m_deltaMicros;

      float m_deltaTime;
      double m_timeScale;
   };


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
      uint32_t m_lastMicros;
      uint32_t m_currentMicros;
      uint32_t m_deltaMicros;
      float m_deltaSeconds;
      float m_timeScale;
   };

   class Cooldown
   {
   public:
      Cooldown(uint32_t durationMicros)
         : m_durationMicros(durationMicros)
      {}

      void updateBy(uint32_t deltaMicros)
      {
         m_timer.updateBy(deltaMicros);
      }
      bool hasElapsed() const
      {
         return m_timer.getCurrentMicros() >= m_durationMicros;
      }
      uint32_t getRemainingMicros() const
      {
         return m_durationMicros - m_timer.getCurrentMicros();
      }
      void reset()
      {
         m_timer.reset(m_durationMicros);
      }

      uint32_t getDurationMicros() const
      {
         return m_durationMicros;
      }
      void setDurationMicros(uint32_t durationMicros)
      {
         m_durationMicros = durationMicros;
      }

   protected:
      Timer m_timer;
      uint32_t m_durationMicros;
   };
}
