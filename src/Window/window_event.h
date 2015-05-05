#pragma once
/********************************************
*  contents:   general window events that the OS code can send to game code
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
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

   struct ButtonKey
   {
      uint8_t id;
      bool isDown;
   };

   typedef ButtonKey KeyboardKey;

   struct KeyboardEvent
   {
      KeyboardKey key;
      uint8_t repeatCount;
      bool firstTimeDown;
   };

   struct MousePosition
   {
      int32_t x;
      int32_t y;
   };

   struct MouseWheel
   {
      int32_t scroll;
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
      uint32_t id;
      float x;
      float y;
      float magnitude;
      float normalized;
   };

   typedef ButtonKey GamepadButton;

   struct GamepadEvent
   {
      uint32_t id;
      union
      {
         GamepadButton button;
         GamepadConnection connection;
         GamepadAxis axis;
      };
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

   static const uint32_t FilenameStringSize = 50;
   struct FileChangeEvent
   {
      FileChangeType action;
      char filename[FilenameStringSize + 1];
   };


   struct WindowEvent
   {
      uint64_t timestamp;
      uint32_t type;
      union
      {
         KeyboardEvent keyboard;
         MouseEvent mouse;
         GamepadEvent gamepad;
         FileChangeEvent fileChange;
      };
   };
}
