#pragma once
/********************************************
*  contents:   general window events that the OS code can send to game code
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   enum class WinMsgType
   {
      Invalid = 0,
      //special case
      WindowHandle,
      //input devices
      KeyboardKey,
      KeyboardText,
      MouseMove,
      MouseButton,
      MouseWheel,
      GamepadConnection,
      GamepadButton,
      GamepadAxis,
      //notifications to game
      LostFocus,
      GainFocus,
      //notification and command
      Close,
      FileChange,
      Size,
      Position,
      //commands to window
      ShowCursor,
      LockCursor,
      RelativeCursor,
      Fullscreen,

      LastEvent
   };

   enum class FileChangeType
   {
      Added,
      Modified,
      Removed,
      RenamedFrom,
      RenamedTo,
      BadData
   };

   enum : u32
   {
      USE_MONITOR_RESOLUTION = 0,
   };

   struct ButtonKey
   {
      u8 id;
      bool isDown;
   };

   typedef ButtonKey KeyboardKey;

   struct KeyboardEvent
   {
      KeyboardKey key;
      u8 repeatCount;
      bool firstTimeDown;
   };

   struct MousePosition
   {
      i32 x;
      i32 y;
   };

   struct MouseWheel
   {
      i32 scroll;
   };

   struct MouseMove
   {
      bool relative;
   };

   typedef ButtonKey MouseButton;


   struct MouseEvent
   {
      MousePosition position;
      union
      {
         MouseButton button;
         MouseWheel wheel;
         MouseMove move;
      };
   };

   struct GamepadConnection
   {
      bool status;
   };

   struct GamepadAxis
   {
      u32 id;
      f32 x;
      f32 y;
      f32 magnitude;
      f32 normalized;
   };

   typedef ButtonKey GamepadButton;

   struct GamepadEvent
   {
      u32 id;
      union
      {
         GamepadButton button;
         GamepadConnection connection;
         GamepadAxis axis;
      };
   };
   
   struct FileChangeEvent
   {
      enum
      {
         NameStringSize = 50 + 1, //+1 for \0
      };
      FileChangeType action;
      char name[NameStringSize];
   };

   struct WinMsg
   {
      u64 timestamp;
      WinMsgType type;
      union
      {
         KeyboardEvent keyboard;
         MouseEvent mouse;
         GamepadEvent gamepad;
         FileChangeEvent fileChange;
         bool fullscreen;
         bool showCursor;
         bool relativeCursor;
         bool lockCursor;
         struct
         {
            u32 x;
            u32 y;
         } screen;
         u64 handle;
      };
   };
}
