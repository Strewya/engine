#pragma once
/********************************************
*  contents:   Utility defines and functions
*  usage:
********************************************/
/******* c++ headers *******/
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
#define Bytes(n) n
#define Kilobytes(n) n*1024
#define Megabytes(n) Kilobytes(n*1024)
#define Gigabytes(n) Megabytes(n*1024)
}
