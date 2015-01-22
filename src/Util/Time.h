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
      CooldownTimer() : CooldownTimer(0)
      {}

      CooldownTimer(uint32_t durationMicros)
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
      float getPercentDone() const
      {
         return static_cast<float>(m_timer.getCurrentMicros()) / static_cast<float>(m_durationMicros);
      }
      void reset()
      {
         m_timer.reset();
      }

      uint32_t getDurationMicros() const
      {
         return m_durationMicros;
      }
      void setDurationMicros(uint32_t durationMicros)
      {
         m_durationMicros = durationMicros;
      }
      const Timer& getTimer() const
      {
         return m_timer;
      }

   protected:
      Timer m_timer;
      uint32_t m_durationMicros;
   };

   class PeriodicTimer
   {
   public:
      PeriodicTimer() : PeriodicTimer(0)
      {}

      PeriodicTimer(uint32_t periodMicros)
         : m_periodMicros(periodMicros)
      {}

      void updateBy(uint32_t deltaMicros)
      {
         m_timer.updateBy(deltaMicros);
      }
      bool hasElapsed() const
      {
         return m_timer.getCurrentMicros() >= m_periodMicros;
      }
      uint32_t getRemainingMicros() const
      {
         return m_periodMicros - m_timer.getCurrentMicros();
      }
      float getPercentDone() const
      {
         return static_cast<float>(m_timer.getCurrentMicros()) / static_cast<float>(m_periodMicros);
      }
      void period()
      {
         m_timer.reset(m_periodMicros);
      }
      void hardReset()
      {
         m_timer.reset();
      }

      uint32_t getPeriodMicros() const
      {
         return m_periodMicros;
      }
      void setPeriodMicros(uint32_t periodMicros)
      {
         m_periodMicros = periodMicros;
      }
      const Timer& getTimer() const
      {
         return m_timer;
      }

   protected:
      Timer m_timer;
      uint32_t m_periodMicros;
   };

   namespace nSeconds
   {
      inline uint32_t toMicros(uint32_t s)
      {
         return s * 1000U * 1000U;
      }
      inline uint32_t toMicros(float s)
      {
         return static_cast<uint32_t>(s*1000.0f) * 1000U;
      }
      inline uint32_t toMilis(uint32_t s)
      {
         return s * 1000U;
      }
      inline uint32_t toMilis(float s)
      {
         return static_cast<uint32_t>(s*1000.0f);
      }
   }
   namespace nMilis
   {
      inline uint32_t toMicros(uint32_t ms)
      {
         return ms * 1000U;
      }
      inline float toSeconds(uint32_t ms)
      {
         return static_cast<float>(ms)*0.001f;
      }
   }
   namespace nMicros
   {
      inline float toSeconds(uint32_t us)
      {
         return static_cast<float>(us)*0.001f*0.001f;
      }
   }
}
