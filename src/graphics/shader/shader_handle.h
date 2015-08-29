#pragma once
/********************************************
*  contents:   pixel and vertex shader handle declarations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/template/resource_handle_template.h"
/******* end header inclusion *******/

namespace core
{
   CORE_RESOURCE_HANDLE(HVertexShader);

   CORE_RESOURCE_HANDLE(HPixelShader);
}
