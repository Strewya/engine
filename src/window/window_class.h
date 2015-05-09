#pragma once
/********************************************
*  contents:   Windows OS specific
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   class WindowClass : public WNDCLASSEX
   {
   private:
      void FillDefaultData();

   public:
      WindowClass(const char* className);
      WindowClass(HINSTANCE hInst, const char* className);
      WindowClass(const std::string& className);
      WindowClass(HINSTANCE hInst, const std::string& className);

      ATOM registerClass();
      const char* getClassName() const;
   };

}