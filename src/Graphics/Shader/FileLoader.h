#pragma once
/********************************************
*  contents:   File loading function
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   bool loadFile(const std::string& filename, std::vector<char>& buffer);
}
