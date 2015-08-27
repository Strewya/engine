#pragma once
/********************************************
*  contents:   general window events that the OS code can send to game code
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "util/types.h"
/******* end header inclusion *******/

namespace core
{
   enum WindowEventType
   {
      WE_INVALID = 0,

      WE_KEYBOARDKEY,
      WE_KEYBOARDTEXT,

      WE_MOUSEMOVE,
      WE_MOUSEBUTTON,
      WE_MOUSEWHEEL,

      WE_GAMEPADCONNECTION,
      WE_GAMEPADBUTTON,
      WE_GAMEPADAXIS,

      WE_CLOSE,
      WE_LOSTFOCUS,
      WE_GAINFOCUS,

      WE_FILECHANGE,

      WE_LASTEVENT
   };

   enum FileChangeType
   {
      FILE_ADDED,
      FILE_MODIFIED,
      FILE_REMOVED,
      FILE_RENAMED_FROM,
      FILE_RENAMED_TO,
      FILE_BADDATA
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
         FilenameStringSize = 50 + 1, //+1 for \0
      };
      FileChangeType action;
      char filename[FilenameStringSize];
   };

   struct WindowEvent
   {
      u64 timestamp;
      u32 type;
      union
      {
         KeyboardEvent keyboard;
         MouseEvent mouse;
         GamepadEvent gamepad;
         FileChangeEvent fileChange;
      };
   };
}
