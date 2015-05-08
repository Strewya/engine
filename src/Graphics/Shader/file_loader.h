#pragma once
/********************************************
*  contents:   function to load a file into byte array
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   bool loadFile(const char* filename, std::vector<char>& buffer);
}
