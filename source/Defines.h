#ifndef DEFINES_H_
#define DEFINES_H_

/*		C++ HEADERS	*/
#include <cassert>
#include <string>

/*		TYPEDEFS	*/
typedef std::string String;
typedef unsigned int uint;
typedef long InstanceID;
typedef unsigned long DWORD;
typedef unsigned long ulong;

/*		CONTANTS	*/
const uint gDefaultDataStoreSize = 64;

/*		MACROS		*/
#define NO_ID 0
#define NOT_FOUND -1

/*		FUNCTIONS	*/

/*		MY HEADERS	*/
#include "Util/Clock.h"
#include "Util/Logger.h"

#endif //DEFINES_H_