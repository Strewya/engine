/*
** Lua binding: s
** Generated automatically by tolua++-1.0.92 on 02/01/14 11:56:23.
*/

#include <stdafx.h>

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include <string>

#include <tolua++.h>

/* Exported function */
TOLUA_API int  tolua_s_open (lua_State* tolua_S);

#include <Util/Dimensional.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Core__Vec2 (lua_State* tolua_S)
{
 Core::Vec2* self = (Core::Vec2*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Core::Vec2");
}

/* get function: x of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_get_Core__Vec2_x
static int tolua_get_Core__Vec2_x(lua_State* tolua_S)
{
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_set_Core__Vec2_x
static int tolua_set_Core__Vec2_x(lua_State* tolua_S)
{
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_get_Core__Vec2_y
static int tolua_get_Core__Vec2_y(lua_State* tolua_S)
{
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_set_Core__Vec2_y
static int tolua_set_Core__Vec2_y(lua_State* tolua_S)
{
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2_new00
static int tolua_s_Core_Vec2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Vec2* tolua_ret = (Core::Vec2*)  Mtolua_new((Core::Vec2)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Vec2");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2_new00_local
static int tolua_s_Core_Vec2_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Vec2* tolua_ret = (Core::Vec2*)  Mtolua_new((Core::Vec2)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2_new01
static int tolua_s_Core_Vec2_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   Core::Vec2* tolua_ret = (Core::Vec2*)  Mtolua_new((Core::Vec2)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Vec2");
  }
 }
 return 1;
tolua_lerror:
 return tolua_s_Core_Vec2_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2_new01_local
static int tolua_s_Core_Vec2_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   Core::Vec2* tolua_ret = (Core::Vec2*)  Mtolua_new((Core::Vec2)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_s_Core_Vec2_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2_set00
static int tolua_s_Core_Vec2_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2__add00
static int tolua_s_Core_Vec2__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Core::Vec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
  const Core::Vec2* b = ((const Core::Vec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Core::Vec2 tolua_ret = (Core::Vec2)  self->operator+(*b);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Core::Vec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Core::Vec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2__sub00
static int tolua_s_Core_Vec2__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Core::Vec2 tolua_ret = (Core::Vec2)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Core::Vec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Core::Vec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_s_Core_Vec2__mul00
static int tolua_s_Core_Vec2__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Vec2* self = (Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Core::Vec2 tolua_ret = (Core::Vec2)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Core::Vec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Core::Vec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Core::Vec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_s_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Core",0);
  tolua_beginmodule(tolua_S,"Core");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vec2","Core::Vec2","",tolua_collect_Core__Vec2);
   #else
   tolua_cclass(tolua_S,"Vec2","Core::Vec2","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vec2");
    tolua_variable(tolua_S,"x",tolua_get_Core__Vec2_x,tolua_set_Core__Vec2_x);
    tolua_variable(tolua_S,"y",tolua_get_Core__Vec2_y,tolua_set_Core__Vec2_y);
    tolua_function(tolua_S,"new",tolua_s_Core_Vec2_new00);
    tolua_function(tolua_S,"new_local",tolua_s_Core_Vec2_new00_local);
    tolua_function(tolua_S,".call",tolua_s_Core_Vec2_new00_local);
    tolua_function(tolua_S,"new",tolua_s_Core_Vec2_new01);
    tolua_function(tolua_S,"new_local",tolua_s_Core_Vec2_new01_local);
    tolua_function(tolua_S,".call",tolua_s_Core_Vec2_new01_local);
    tolua_function(tolua_S,"set",tolua_s_Core_Vec2_set00);
    tolua_function(tolua_S,".add",tolua_s_Core_Vec2__add00);
    tolua_function(tolua_S,".sub",tolua_s_Core_Vec2__sub00);
    tolua_function(tolua_S,".mul",tolua_s_Core_Vec2__mul00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_s (lua_State* tolua_S) {
 return tolua_s_open(tolua_S);
};
#endif

