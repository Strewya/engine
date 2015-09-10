//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/gamepad_handler.h"
/******* c++ headers *******/
#include <vector>
/******* extra headers *******/
#include "input/gamepad.h"
#include "utility/time/time_unit_conversions.h"
#include "utility/geometry/vec2.h"
#include "utility/communication_buffer.h"
#include "window/window_message.h"
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

   core_internal void writeButton(CommunicationBuffer* buffer, u8 id, u8 button, bool isDown)
   {
      WinMsg msg{};
      msg.type = WinMsgType::GamepadButton;
      msg.gamepad.id = id;
      msg.gamepad.button.id = button;
      msg.gamepad.button.isDown = isDown;
      buffer->writeEvent(msg);
   }

   core_internal void writeAxis(CommunicationBuffer* buffer, u8 id, u8 axis, f32 x, f32 y, f32 mag, f32 norm)
   {
      WinMsg msg{};
      msg.type = WinMsgType::GamepadAxis;
      msg.gamepad.id = id;
      msg.gamepad.axis.id = axis;
      msg.gamepad.axis.x = x;
      msg.gamepad.axis.y = y;
      msg.gamepad.axis.magnitude = mag;
      msg.gamepad.axis.normalized = norm;
      buffer->writeEvent(msg);
   }

   core_internal void writeConnection(CommunicationBuffer* buffer, u8 id, bool connected)
   {
      WinMsg msg{};
      msg.type = WinMsgType::GamepadConnection;
      msg.gamepad.id = id;
      msg.gamepad.connection.status = connected;
   }

   core_internal void handleButton(CommunicationBuffer* buffer, u8 id, XINPUT_STATE& oldState, XINPUT_STATE& newState, u32 xiButton, Gamepad::Key key)
   {
      if( (oldState.Gamepad.wButtons & xiButton) != (newState.Gamepad.wButtons & xiButton) )
      {
         writeButton(buffer, id, key, (newState.Gamepad.wButtons & xiButton) != 0);
      }
   }

   struct AnalogData
   {
      f32 value;
      f32 normalized;
   };

   core_internal AnalogData calcAnalogData(f32 inputValue, u32 deadzone, u32 maxValue)
   {
      AnalogData result{inputValue, 0};
      if( result.value > deadzone )
      {
         if( result.value > maxValue )
         {
            result.value = (f32)maxValue;
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

   core_internal void handleTrigger(CommunicationBuffer* buffer, u8 id, u32 oldValue, u32 newValue, u32 threshold, u32 maxValue, Gamepad::Key axis)
   {
      f32 on = 0.8f;
      f32 off = 0.2f;

      if( newValue != oldValue )
      {
         auto newData = calcAnalogData((f32)newValue, threshold, maxValue);
         auto oldData = calcAnalogData((f32)oldValue, threshold, maxValue);
         writeAxis(buffer, id, axis, 0, 0, newData.value, newData.normalized);

         if( (oldData.normalized < on && newData.normalized >= on) ||
            (oldData.normalized > off && newData.normalized <= off) )
         {
            writeButton(buffer, id, axis, newData.normalized >= on);
         }
      }
   }

   void handleStick(CommunicationBuffer* buffer, u8 id, v2 oldPos, v2 newPos, u32 threshold, u32 maxValue, Gamepad::Key axis)
   {
      f32 on = 0.8f;
      f32 off = 0.2f;

      if( newPos != oldPos )
      {
         auto newData = calcAnalogData(vec2::length(newPos), threshold, maxValue);
         auto oldData = calcAnalogData(vec2::length(oldPos), threshold, maxValue);
         newPos = vec2::normalize(newPos);
         writeAxis(buffer, id, axis, newPos.x, newPos.y, newData.value, newData.normalized);

         if( (oldData.normalized < on && newData.normalized >= on) ||
            (oldData.normalized > off && newData.normalized <= off) )
         {
            u8 button = 0;
            if( newPos.x >= on )
               button = Gamepad::DPadRight;
            else if( newPos.x <= -on )
               button = Gamepad::DPadLeft;
            else if( newPos.y <= -on )
               button = Gamepad::DPadDown;
            else if( newPos.y >= on )
               button = Gamepad::DPadUp;
            writeButton(buffer, id, button, newData.normalized >= on);
         }
      }
   }

   bool handleConnection(CommunicationBuffer* buffer, u8 id, u32 result, bool oldConnectionState)
   {
      if( (result == ERROR_SUCCESS && oldConnectionState == false) || (result == ERROR_DEVICE_NOT_CONNECTED && oldConnectionState == true) )
      {
         oldConnectionState = !oldConnectionState;
         writeConnection(buffer, id, oldConnectionState);
      }
      return oldConnectionState;
   }

   void GamepadHandler::handle(CommunicationBuffer* buffer)
   {
      u64 currentTime = Clock::getRealTimeMicros();
      for( u8 i = 0; i < MAX_GAMEPADS; ++i )
      {
         if( m_gamepadConnected[i] || (currentTime >= m_gamepadLastUpdateTime[i] + m_unconnectedReadDelay) )
         {
            XINPUT_STATE& state = m_gamepadState[i];
            XINPUT_STATE oldState = state;
            auto result = XInputGetState(i, &state);

            m_gamepadLastUpdateTime[i] = currentTime;
            m_gamepadConnected[i] = handleConnection(buffer, i, result, m_gamepadConnected[i]);
            if( m_gamepadConnected[i] && state.dwPacketNumber != oldState.dwPacketNumber )
            {
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_DPAD_UP, Gamepad::DPadUp);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_DPAD_DOWN, Gamepad::DPadDown);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_DPAD_LEFT, Gamepad::DPadLeft);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_DPAD_RIGHT, Gamepad::DPadRight);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_START, Gamepad::Start);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_BACK, Gamepad::Back);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_LEFT_THUMB, Gamepad::LeftThumb);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_RIGHT_THUMB, Gamepad::RightThumb);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_LEFT_SHOULDER, Gamepad::LeftShoulder);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_RIGHT_SHOULDER, Gamepad::RightShoulder);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_A, Gamepad::BottomButton);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_B, Gamepad::RightButton);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_X, Gamepad::LeftButton);
               handleButton(buffer, i, oldState, state, XINPUT_GAMEPAD_Y, Gamepad::TopButton);


               /*** TRIGGERS ***/
               handleTrigger(buffer, i, oldState.Gamepad.bLeftTrigger, state.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 0xff, Gamepad::LeftTrigger);
               handleTrigger(buffer, i, oldState.Gamepad.bRightTrigger, state.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 0xff, Gamepad::RightTrigger);

               /*** THUMB STICKS ***/
               handleStick(buffer, i, {oldState.Gamepad.sThumbLX, oldState.Gamepad.sThumbLY}, {state.Gamepad.sThumbLX, state.Gamepad.sThumbLY},
                           XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0x7fff, Gamepad::LeftStick);
               handleStick(buffer, i, {oldState.Gamepad.sThumbRX, oldState.Gamepad.sThumbRY}, {state.Gamepad.sThumbRX, state.Gamepad.sThumbRY},
                           XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0x7fff, Gamepad::RightStick);
            }
         }
      }
   }
}
