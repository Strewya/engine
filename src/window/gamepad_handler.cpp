//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/gamepad_handler.h"
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* extra headers *******/
#include "input/gamepad.h"
#include "util/time/time_unit_conversions.h"
#include "util/geometry/vec2.h"
#include "window/window_event.h"
/******* end headers *******/

namespace core
{
   GamepadHandler::GamepadHandler()
      : m_unconnectedReadDelay(milisToMicros(200))
   {
      memset(m_gamepadState, 0, sizeof(m_gamepadState));
      memset(m_gamepadConnected, 0, sizeof(m_gamepadConnected));
      memset(m_gamepadLastUpdateTime, 0, sizeof(m_gamepadLastUpdateTime));
   }

   void writeButton(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, uint8_t button, bool isDown)
   {
      events.emplace_back();
      events.back().type = WE_GAMEPADBUTTON;
      events.back().timestamp = timestamp;
      events.back().gamepad.id = id;
      events.back().gamepad.button.id = button;
      events.back().gamepad.button.isDown = isDown;
   }

   void writeAxis(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, uint8_t axis, float x, float y, float mag, float norm)
   {
      events.emplace_back();
      events.back().type = WE_GAMEPADAXIS;
      events.back().timestamp = timestamp;
      events.back().gamepad.id = id;
      events.back().gamepad.axis.id = axis;
      events.back().gamepad.axis.x = x;
      events.back().gamepad.axis.y = y;
      events.back().gamepad.axis.magnitude = mag;
      events.back().gamepad.axis.normalized = norm;
   }

   void writeConnection(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, bool connected)
   {
      events.emplace_back();
      events.back().type = WE_GAMEPADCONNECTION;
      events.back().timestamp = timestamp;
      events.back().gamepad.id = id;
      events.back().gamepad.connection.status = connected;
   }

   void handleButton(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, XINPUT_STATE& oldState, XINPUT_STATE& newState, uint32_t xiButton, Gamepad::Key key)
   {
      if( (oldState.Gamepad.wButtons & xiButton) != (newState.Gamepad.wButtons & xiButton) )
      {
         writeButton(events, timestamp, id, key, (newState.Gamepad.wButtons & xiButton) != 0);
      }
   }

   struct AnalogData
   {
      float value;
      float normalized;
   };

   AnalogData calcAnalogData(float inputValue, uint32_t deadzone, uint32_t maxValue)
   {
      AnalogData result{inputValue, 0};
      if( result.value > deadzone )
      {
         if( result.value > maxValue )
         {
            result.value = (float)maxValue;
         }
         result.value -= deadzone;
         result.normalized = result.value / (maxValue - deadzone);
      }
      else
      {
         result.value = 0;
      }
      return result;
   }

   void handleTrigger(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, uint32_t oldValue, uint32_t newValue, uint32_t threshold, uint32_t maxValue, Gamepad::Key axis)
   {
      float on = 0.8f;
      float off = 0.2f;

      if( newValue != oldValue )
      {
         auto newData = calcAnalogData((float)newValue, threshold, maxValue);
         auto oldData = calcAnalogData((float)oldValue, threshold, maxValue);
         writeAxis(events, timestamp, id, axis, 0, 0, newData.value, newData.normalized);

         if( (oldData.normalized < on && newData.normalized >= on) ||
            (oldData.normalized > off && newData.normalized <= off) )
         {
            writeButton(events, timestamp, id, axis, newData.normalized >= on);
         }
      }
   }

   void handleStick(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, Vec2 oldPos, Vec2 newPos, uint32_t threshold, uint32_t maxValue, Gamepad::Key axis)
   {
      float on = 0.8f;
      float off = 0.2f;

      if( newPos != oldPos )
      {
         auto newData = calcAnalogData(vec2::length(newPos), threshold, maxValue);
         auto oldData = calcAnalogData(vec2::length(oldPos), threshold, maxValue);
         newPos = vec2::normalize(newPos);
         writeAxis(events, timestamp, id, axis, newPos.x, newPos.y, newData.value, newData.normalized);

         if( (oldData.normalized < on && newData.normalized >= on) ||
            (oldData.normalized > off && newData.normalized <= off) )
         {
            uint8_t button = 0;
            if( newPos.x >= on )
               button = Gamepad::DPadRight;
            else if( newPos.x <= -on )
               button = Gamepad::DPadLeft;
            else if( newPos.y <= -on )
               button = Gamepad::DPadDown;
            else if( newPos.y >= on )
               button = Gamepad::DPadUp;
            writeButton(events, timestamp, id, button, newData.normalized >= on);
         }
      }
   }

   bool handleConnection(std::vector<WindowEvent>& events, uint64_t timestamp, uint8_t id, uint32_t result, bool oldConnectionState)
   {
      if( (result == ERROR_SUCCESS && oldConnectionState == false) || (result == ERROR_DEVICE_NOT_CONNECTED && oldConnectionState == true) )
      {
         oldConnectionState = !oldConnectionState;
         writeConnection(events, timestamp, id, oldConnectionState);
      }
      return oldConnectionState;
   }

   std::vector<WindowEvent> GamepadHandler::handle(uint64_t currentTime)
   {
      std::vector<WindowEvent> events;
      for( uint8_t i = 0; i < MAX_GAMEPADS; ++i )
      {
         if( m_gamepadConnected[i] || (currentTime >= m_gamepadLastUpdateTime[i] + m_unconnectedReadDelay) )
         {
            XINPUT_STATE& state = m_gamepadState[i];
            XINPUT_STATE oldState = state;
            auto result = XInputGetState(i, &state);

            m_gamepadLastUpdateTime[i] = currentTime;
            m_gamepadConnected[i] = handleConnection(events, currentTime, i, result, m_gamepadConnected[i]);
            if( m_gamepadConnected[i] && state.dwPacketNumber != oldState.dwPacketNumber )
            {
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_DPAD_UP, Gamepad::DPadUp);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_DPAD_DOWN, Gamepad::DPadDown);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_DPAD_LEFT, Gamepad::DPadLeft);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_DPAD_RIGHT, Gamepad::DPadRight);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_START, Gamepad::Start);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_BACK, Gamepad::Back);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_LEFT_THUMB, Gamepad::LeftThumb);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_RIGHT_THUMB, Gamepad::RightThumb);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_LEFT_SHOULDER, Gamepad::LeftShoulder);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_RIGHT_SHOULDER, Gamepad::RightShoulder);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_A, Gamepad::BottomButton);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_B, Gamepad::RightButton);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_X, Gamepad::LeftButton);
               handleButton(events, currentTime, i, oldState, state, XINPUT_GAMEPAD_Y, Gamepad::TopButton);


               /*** TRIGGERS ***/
               handleTrigger(events, currentTime, i, oldState.Gamepad.bLeftTrigger, state.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 0xff, Gamepad::LeftTrigger);
               handleTrigger(events, currentTime, i, oldState.Gamepad.bRightTrigger, state.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 0xff, Gamepad::RightTrigger);

               /*** THUMB STICKS ***/
               handleStick(events, currentTime, i, {oldState.Gamepad.sThumbLX, oldState.Gamepad.sThumbLY}, {state.Gamepad.sThumbLX, state.Gamepad.sThumbLY},
                           XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0x7fff, Gamepad::LeftStick);
               handleStick(events, currentTime, i, {oldState.Gamepad.sThumbRX, oldState.Gamepad.sThumbRY}, {state.Gamepad.sThumbRX, state.Gamepad.sThumbRY},
                           XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0x7fff, Gamepad::RightStick);
            }
         }
      }
      return events;
   }
}
