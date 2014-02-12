/*
** Lua binding: core
** Generated automatically by tolua++-1.0.92 on 02/12/14 16:25:49.
*/

#include <stdafx.h>

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include <string>

#include <tolua++.h>

/* Exported function */
TOLUA_API int  tolua_core_open (lua_State* tolua_S);

#include <Util/Color.h>
#include <Util/Dimensional.h>
#include <Util/Random.h>
#include <Util/Time.h>
#include <Util/Transform.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
#include <Games/Hedgehog/HedgehogGame.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Core__Color (lua_State* tolua_S)
{
 Core::Color* self = (Core::Color*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Core__Random (lua_State* tolua_S)
{
 Core::Random* self = (Core::Random*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Core__Time (lua_State* tolua_S)
{
 Core::Time* self = (Core::Time*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Core__Vec2 (lua_State* tolua_S)
{
 Core::Vec2* self = (Core::Vec2*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Core__Transform (lua_State* tolua_S)
{
 Core::Transform* self = (Core::Transform*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Core::Color");
 tolua_usertype(tolua_S,"Core::Time");
 tolua_usertype(tolua_S,"Core::Vec2");
 tolua_usertype(tolua_S,"Core::WindowEvent");
 tolua_usertype(tolua_S,"Core::KeyboardEvent");
 tolua_usertype(tolua_S,"Core::MouseWheelEvent");
 tolua_usertype(tolua_S,"Core::FileChangeEvent");
 tolua_usertype(tolua_S,"Core::Random");
 tolua_usertype(tolua_S,"Core::Window");
 tolua_usertype(tolua_S,"Core::MouseMoveEvent");
 tolua_usertype(tolua_S,"Core::HedgehogGame");
 tolua_usertype(tolua_S,"Core::Transform");
 tolua_usertype(tolua_S,"Core::MouseButtonEvent");
}

/* get function: r of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_get_Core__Color_r
static int tolua_get_Core__Color_r(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->r);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: r of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_set_Core__Color_r
static int tolua_set_Core__Color_r(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->r = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_get_Core__Color_g
static int tolua_get_Core__Color_g(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->g);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_set_Core__Color_g
static int tolua_set_Core__Color_g(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->g = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_get_Core__Color_b
static int tolua_get_Core__Color_b(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->b);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_set_Core__Color_b
static int tolua_set_Core__Color_b(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: a of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_get_Core__Color_a
static int tolua_get_Core__Color_a(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->a);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: a of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_set_Core__Color_a
static int tolua_set_Core__Color_a(lua_State* tolua_S)
{
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->a = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_core_Core_Color_new00
static int tolua_core_Core_Color_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Color* tolua_ret = (Core::Color*)  Mtolua_new((Core::Color)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Color");
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

/* method: new_local of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_core_Core_Color_new00_local
static int tolua_core_Core_Color_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Color* tolua_ret = (Core::Color*)  Mtolua_new((Core::Color)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Color");
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

/* method: new of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_core_Core_Color_new01
static int tolua_core_Core_Color_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   Core::Color* tolua_ret = (Core::Color*)  Mtolua_new((Core::Color)(r,g,b));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_core_Core_Color_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_core_Core_Color_new01_local
static int tolua_core_Core_Color_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   Core::Color* tolua_ret = (Core::Color*)  Mtolua_new((Core::Color)(r,g,b));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Color");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_core_Core_Color_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  Core::Color */
#ifndef TOLUA_DISABLE_tolua_core_Core_Color_set00
static int tolua_core_Core_Color_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Color* self = (Core::Color*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(r,g,b);
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
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2_new00
static int tolua_core_Core_Vec2_new00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2_new00_local
static int tolua_core_Core_Vec2_new00_local(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2_new01
static int tolua_core_Core_Vec2_new01(lua_State* tolua_S)
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
 return tolua_core_Core_Vec2_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2_new01_local
static int tolua_core_Core_Vec2_new01_local(lua_State* tolua_S)
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
 return tolua_core_Core_Vec2_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2_set00
static int tolua_core_Core_Vec2_set00(lua_State* tolua_S)
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
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2__add00
static int tolua_core_Core_Vec2__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Vec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Core::Vec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Vec2* self = (const Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
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
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2__sub00
static int tolua_core_Core_Vec2__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Vec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Vec2* self = (const Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator- of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2__sub01
static int tolua_core_Core_Vec2__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Vec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Core::Vec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Core::Vec2* self = (const Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
  const Core::Vec2* v = ((const Core::Vec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Core::Vec2 tolua_ret = (Core::Vec2)  self->operator-(*v);
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
tolua_lerror:
 return tolua_core_Core_Vec2__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2__mul00
static int tolua_core_Core_Vec2__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Vec2* self = (const Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator/ of class  Core::Vec2 */
#ifndef TOLUA_DISABLE_tolua_core_Core_Vec2__div00
static int tolua_core_Core_Vec2__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Vec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Vec2* self = (const Core::Vec2*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'", NULL);
#endif
  {
   Core::Vec2 tolua_ret = (Core::Vec2)  self->operator/(f);
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
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_new00
static int tolua_core_Core_Random_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Random* tolua_ret = (Core::Random*)  Mtolua_new((Core::Random)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Random");
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

/* method: new_local of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_new00_local
static int tolua_core_Core_Random_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Random* tolua_ret = (Core::Random*)  Mtolua_new((Core::Random)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Random");
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

/* method: new of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_new01
static int tolua_core_Core_Random_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int seed = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   Core::Random* tolua_ret = (Core::Random*)  Mtolua_new((Core::Random)(seed));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Random");
  }
 }
 return 1;
tolua_lerror:
 return tolua_core_Core_Random_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_new01_local
static int tolua_core_Core_Random_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int seed = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   Core::Random* tolua_ret = (Core::Random*)  Mtolua_new((Core::Random)(seed));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Random");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_core_Core_Random_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: reseed of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_reseed00
static int tolua_core_Core_Random_reseed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Random* self = (Core::Random*)  tolua_tousertype(tolua_S,1,0);
  unsigned int seed = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reseed'", NULL);
#endif
  {
   self->reseed(seed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reseed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: randInt of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_randInt00
static int tolua_core_Core_Random_randInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Random* self = (Core::Random*)  tolua_tousertype(tolua_S,1,0);
   int lowerBound = ((  int)  tolua_tonumber(tolua_S,2,0));
   int upperBound = ((  int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'randInt'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->randInt(lowerBound,upperBound);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'randInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: randFloat of class  Core::Random */
#ifndef TOLUA_DISABLE_tolua_core_Core_Random_randFloat00
static int tolua_core_Core_Random_randFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Random",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Random* self = (Core::Random*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'randFloat'", NULL);
#endif
  {
   float tolua_ret = (float)  self->randFloat();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'randFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: STOP_TIME of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_get_Core__Time_STOP_TIME
static int tolua_get_Core__Time_STOP_TIME(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Core::Time::STOP_TIME);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: NORMAL_TIME of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_get_Core__Time_NORMAL_TIME
static int tolua_get_Core__Time_NORMAL_TIME(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Core::Time::NORMAL_TIME);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: microsFromSeconds of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_microsFromSeconds00
static int tolua_core_Core_Time_microsFromSeconds00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int sec = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned long long tolua_ret = (unsigned long long)  Core::Time::microsFromSeconds(sec);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'microsFromSeconds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: microsFromMilis of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_microsFromMilis00
static int tolua_core_Core_Time_microsFromMilis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int mili = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned long long tolua_ret = (unsigned long long)  Core::Time::microsFromMilis(mili);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'microsFromMilis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_new00
static int tolua_core_Core_Time_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Time* tolua_ret = (Core::Time*)  Mtolua_new((Core::Time)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Time");
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

/* method: new_local of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_new00_local
static int tolua_core_Core_Time_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Time* tolua_ret = (Core::Time*)  Mtolua_new((Core::Time)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Time");
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

/* method: update of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_update00
static int tolua_core_Core_Time_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Time* self = (Core::Time*)  tolua_tousertype(tolua_S,1,0);
  double virtualTimeScale = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update(virtualTimeScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateBy of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_updateBy00
static int tolua_core_Core_Time_updateBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Time* self = (Core::Time*)  tolua_tousertype(tolua_S,1,0);
  unsigned long long delta = (( unsigned long long)  tolua_tonumber(tolua_S,2,0));
  double virtualTimeScale = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateBy'", NULL);
#endif
  {
   self->updateBy(delta,virtualTimeScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFixedStepUpdateCount of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getFixedStepUpdateCount00
static int tolua_core_Core_Time_getFixedStepUpdateCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Time",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Time* self = (Core::Time*)  tolua_tousertype(tolua_S,1,0);
  unsigned long long frameTime = (( unsigned long long)  tolua_tonumber(tolua_S,2,0));
  float ratio = ((float)  tolua_tonumber(tolua_S,3,0));
  unsigned long long remainderTime = (( unsigned long long)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFixedStepUpdateCount'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->getFixedStepUpdateCount(frameTime,ratio,remainderTime);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)ratio);
   tolua_pushnumber(tolua_S,(lua_Number)remainderTime);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFixedStepUpdateCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLastRealTimeMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getLastRealTimeMicros00
static int tolua_core_Core_Time_getLastRealTimeMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLastRealTimeMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getLastRealTimeMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLastRealTimeMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRealTimeMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getRealTimeMicros00
static int tolua_core_Core_Time_getRealTimeMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRealTimeMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getRealTimeMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRealTimeMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getCurMicros00
static int tolua_core_Core_Time_getCurMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getCurMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDeltaMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getDeltaMicros00
static int tolua_core_Core_Time_getDeltaMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDeltaMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getDeltaMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDeltaMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDeltaTime of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getDeltaTime00
static int tolua_core_Core_Time_getDeltaTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDeltaTime'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getDeltaTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDeltaTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVirtCurMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getVirtCurMicros00
static int tolua_core_Core_Time_getVirtCurMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVirtCurMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getVirtCurMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVirtCurMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVirtDeltaMicros of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getVirtDeltaMicros00
static int tolua_core_Core_Time_getVirtDeltaMicros00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVirtDeltaMicros'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->getVirtDeltaMicros();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVirtDeltaMicros'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVirtDeltaTime of class  Core::Time */
#ifndef TOLUA_DISABLE_tolua_core_Core_Time_getVirtDeltaTime00
static int tolua_core_Core_Time_getVirtDeltaTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Time",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Time* self = (const Core::Time*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVirtDeltaTime'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getVirtDeltaTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVirtDeltaTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_core_Core_Transform_new00
static int tolua_core_Core_Transform_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Transform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Transform* tolua_ret = (Core::Transform*)  Mtolua_new((Core::Transform)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Transform");
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

/* method: new_local of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_core_Core_Transform_new00_local
static int tolua_core_Core_Transform_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Core::Transform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Core::Transform* tolua_ret = (Core::Transform*)  Mtolua_new((Core::Transform)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Core::Transform");
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

/* get function: position of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_get_Core__Transform_position
static int tolua_get_Core__Transform_position(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->position,"Core::Vec2");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_set_Core__Transform_position
static int tolua_set_Core__Transform_position(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::Vec2",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = *((Core::Vec2*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: scale of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_get_Core__Transform_scale
static int tolua_get_Core__Transform_scale(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scale'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->scale,"Core::Vec2");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: scale of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_set_Core__Transform_scale
static int tolua_set_Core__Transform_scale(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scale'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::Vec2",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->scale = *((Core::Vec2*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: rotation of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_get_Core__Transform_rotation
static int tolua_get_Core__Transform_rotation(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotation'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->rotation);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: rotation of class  Core::Transform */
#ifndef TOLUA_DISABLE_tolua_set_Core__Transform_rotation
static int tolua_set_Core__Transform_rotation(lua_State* tolua_S)
{
  Core::Transform* self = (Core::Transform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotation'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->rotation = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_keyCode of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__KeyboardEvent_unsigned_m_keyCode
static int tolua_get_Core__KeyboardEvent_unsigned_m_keyCode(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_keyCode'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_keyCode);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_keyCode of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__KeyboardEvent_unsigned_m_keyCode
static int tolua_set_Core__KeyboardEvent_unsigned_m_keyCode(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_keyCode'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_keyCode = (( unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_repeat of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__KeyboardEvent_unsigned_m_repeat
static int tolua_get_Core__KeyboardEvent_unsigned_m_repeat(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_repeat'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_repeat);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_repeat of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__KeyboardEvent_unsigned_m_repeat
static int tolua_set_Core__KeyboardEvent_unsigned_m_repeat(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_repeat'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_repeat = (( unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_isDown of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__KeyboardEvent_m_isDown
static int tolua_get_Core__KeyboardEvent_m_isDown(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isDown'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->m_isDown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_isDown of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__KeyboardEvent_m_isDown
static int tolua_set_Core__KeyboardEvent_m_isDown(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isDown'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_isDown = (tolua_toboolean(tolua_S,2,0) == 1)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_previouslyDown of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__KeyboardEvent_m_previouslyDown
static int tolua_get_Core__KeyboardEvent_m_previouslyDown(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_previouslyDown'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->m_previouslyDown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_previouslyDown of class  Core::KeyboardEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__KeyboardEvent_m_previouslyDown
static int tolua_set_Core__KeyboardEvent_m_previouslyDown(lua_State* tolua_S)
{
  Core::KeyboardEvent* self = (Core::KeyboardEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_previouslyDown'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_previouslyDown = (tolua_toboolean(tolua_S,2,0) == 1)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_x of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseMoveEvent_m_x
static int tolua_get_Core__MouseMoveEvent_m_x(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_x of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseMoveEvent_m_x
static int tolua_set_Core__MouseMoveEvent_m_x(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_x = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_y of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseMoveEvent_m_y
static int tolua_get_Core__MouseMoveEvent_m_y(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_y of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseMoveEvent_m_y
static int tolua_set_Core__MouseMoveEvent_m_y(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_y = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_isRelative of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseMoveEvent_m_isRelative
static int tolua_get_Core__MouseMoveEvent_m_isRelative(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isRelative'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->m_isRelative);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_isRelative of class  Core::MouseMoveEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseMoveEvent_m_isRelative
static int tolua_set_Core__MouseMoveEvent_m_isRelative(lua_State* tolua_S)
{
  Core::MouseMoveEvent* self = (Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isRelative'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_isRelative = (tolua_toboolean(tolua_S,2,0) == 1)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_x of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseButtonEvent_m_x
static int tolua_get_Core__MouseButtonEvent_m_x(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_x of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseButtonEvent_m_x
static int tolua_set_Core__MouseButtonEvent_m_x(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_x = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_y of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseButtonEvent_m_y
static int tolua_get_Core__MouseButtonEvent_m_y(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_y of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseButtonEvent_m_y
static int tolua_set_Core__MouseButtonEvent_m_y(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_y = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_button of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseButtonEvent_unsigned_m_button
static int tolua_get_Core__MouseButtonEvent_unsigned_m_button(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_button'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_button);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_button of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseButtonEvent_unsigned_m_button
static int tolua_set_Core__MouseButtonEvent_unsigned_m_button(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_button'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_button = (( unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_clicks of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseButtonEvent_unsigned_m_clicks
static int tolua_get_Core__MouseButtonEvent_unsigned_m_clicks(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_clicks'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_clicks);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_clicks of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseButtonEvent_unsigned_m_clicks
static int tolua_set_Core__MouseButtonEvent_unsigned_m_clicks(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_clicks'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_clicks = (( unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_isDown of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseButtonEvent_m_isDown
static int tolua_get_Core__MouseButtonEvent_m_isDown(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isDown'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->m_isDown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_isDown of class  Core::MouseButtonEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseButtonEvent_m_isDown
static int tolua_set_Core__MouseButtonEvent_m_isDown(lua_State* tolua_S)
{
  Core::MouseButtonEvent* self = (Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_isDown'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_isDown = (tolua_toboolean(tolua_S,2,0) == 1)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_x of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseWheelEvent_m_x
static int tolua_get_Core__MouseWheelEvent_m_x(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_x of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseWheelEvent_m_x
static int tolua_set_Core__MouseWheelEvent_m_x(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_x = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_y of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseWheelEvent_m_y
static int tolua_get_Core__MouseWheelEvent_m_y(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_y of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseWheelEvent_m_y
static int tolua_set_Core__MouseWheelEvent_m_y(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_y = ((  int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_scroll of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__MouseWheelEvent_unsigned_m_scroll
static int tolua_get_Core__MouseWheelEvent_unsigned_m_scroll(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_scroll'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_scroll);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_scroll of class  Core::MouseWheelEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__MouseWheelEvent_unsigned_m_scroll
static int tolua_set_Core__MouseWheelEvent_unsigned_m_scroll(lua_State* tolua_S)
{
  Core::MouseWheelEvent* self = (Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_scroll'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_scroll = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_index of class  Core::FileChangeEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__FileChangeEvent_unsigned_m_index
static int tolua_get_Core__FileChangeEvent_unsigned_m_index(lua_State* tolua_S)
{
  Core::FileChangeEvent* self = (Core::FileChangeEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_index of class  Core::FileChangeEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__FileChangeEvent_unsigned_m_index
static int tolua_set_Core__FileChangeEvent_unsigned_m_index(lua_State* tolua_S)
{
  Core::FileChangeEvent* self = (Core::FileChangeEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_index = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_type of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_unsigned_m_type
static int tolua_get_Core__WindowEvent_unsigned_m_type(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_type of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_unsigned_m_type
static int tolua_set_Core__WindowEvent_unsigned_m_type(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_type = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_timestamp of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_unsigned_m_timestamp
static int tolua_get_Core__WindowEvent_unsigned_m_timestamp(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_timestamp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_timestamp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_timestamp of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_unsigned_m_timestamp
static int tolua_set_Core__WindowEvent_unsigned_m_timestamp(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_timestamp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_timestamp = (( unsigned long long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_keyboard of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_m_keyboard
static int tolua_get_Core__WindowEvent_m_keyboard(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_keyboard'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_keyboard,"Core::KeyboardEvent");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_keyboard of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_m_keyboard
static int tolua_set_Core__WindowEvent_m_keyboard(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_keyboard'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::KeyboardEvent",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_keyboard = *((Core::KeyboardEvent*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_mouseMove of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_m_mouseMove
static int tolua_get_Core__WindowEvent_m_mouseMove(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseMove'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_mouseMove,"Core::MouseMoveEvent");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_mouseMove of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_m_mouseMove
static int tolua_set_Core__WindowEvent_m_mouseMove(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseMove'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::MouseMoveEvent",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_mouseMove = *((Core::MouseMoveEvent*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_mouseButton of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_m_mouseButton
static int tolua_get_Core__WindowEvent_m_mouseButton(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseButton'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_mouseButton,"Core::MouseButtonEvent");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_mouseButton of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_m_mouseButton
static int tolua_set_Core__WindowEvent_m_mouseButton(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseButton'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::MouseButtonEvent",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_mouseButton = *((Core::MouseButtonEvent*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_mouseWheel of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_m_mouseWheel
static int tolua_get_Core__WindowEvent_m_mouseWheel(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseWheel'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_mouseWheel,"Core::MouseWheelEvent");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_mouseWheel of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_m_mouseWheel
static int tolua_set_Core__WindowEvent_m_mouseWheel(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_mouseWheel'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::MouseWheelEvent",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_mouseWheel = *((Core::MouseWheelEvent*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_fileChange of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_get_Core__WindowEvent_m_fileChange
static int tolua_get_Core__WindowEvent_m_fileChange(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fileChange'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_fileChange,"Core::FileChangeEvent");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_fileChange of class  Core::WindowEvent */
#ifndef TOLUA_DISABLE_tolua_set_Core__WindowEvent_m_fileChange
static int tolua_set_Core__WindowEvent_m_fileChange(lua_State* tolua_S)
{
  Core::WindowEvent* self = (Core::WindowEvent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fileChange'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Core::FileChangeEvent",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fileChange = *((Core::FileChangeEvent*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: showMessagebox of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_showMessagebox00
static int tolua_core_Core_Window_showMessagebox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
  const char* title = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* text = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showMessagebox'", NULL);
#endif
  {
   self->showMessagebox(title,text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showMessagebox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resize of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_resize00
static int tolua_core_Core_Window_resize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int x = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int y = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resize'", NULL);
#endif
  {
   self->resize(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: move of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_move00
static int tolua_core_Core_Window_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
   int x = ((  int)  tolua_tonumber(tolua_S,2,0));
   int y = ((  int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'move'", NULL);
#endif
  {
   self->move(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showCursor of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_showCursor00
static int tolua_core_Core_Window_showCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
  bool isShown = (tolua_toboolean(tolua_S,2,0) == 1);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showCursor'", NULL);
#endif
  {
   self->showCursor(isShown);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showCursor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFullscreen of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_setFullscreen00
static int tolua_core_Core_Window_setFullscreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
  bool isFullscreen = (tolua_toboolean(tolua_S,2,0) == 1);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFullscreen'", NULL);
#endif
  {
   self->setFullscreen(isFullscreen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFullscreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFileChangeDelay of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_setFileChangeDelay00
static int tolua_core_Core_Window_setFileChangeDelay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Core::Window",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Core::Window* self = (Core::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int delay = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFileChangeDelay'", NULL);
#endif
  {
   self->setFileChangeDelay(delay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFileChangeDelay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPositionX of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_getPositionX00
static int tolua_core_Core_Window_getPositionX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPositionX'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->getPositionX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPositionX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPositionY of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_getPositionY00
static int tolua_core_Core_Window_getPositionY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPositionY'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->getPositionY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPositionY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSizeX of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_getSizeX00
static int tolua_core_Core_Window_getSizeX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSizeX'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->getSizeX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSizeX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSizeY of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_getSizeY00
static int tolua_core_Core_Window_getSizeY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSizeY'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->getSizeY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSizeY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCursorShown of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_isCursorShown00
static int tolua_core_Core_Window_isCursorShown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCursorShown'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCursorShown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCursorShown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFullscreen of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_isFullscreen00
static int tolua_core_Core_Window_isFullscreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFullscreen'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isFullscreen();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFullscreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileChangeDelay of class  Core::Window */
#ifndef TOLUA_DISABLE_tolua_core_Core_Window_getFileChangeDelay00
static int tolua_core_Core_Window_getFileChangeDelay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Core::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Core::Window* self = (const Core::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileChangeDelay'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->getFileChangeDelay();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileChangeDelay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_logicTimer of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_get_Core__HedgehogGame_m_logicTimer
static int tolua_get_Core__HedgehogGame_m_logicTimer(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_logicTimer'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_logicTimer,"const Core::Time");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_renderTimer of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_get_Core__HedgehogGame_m_renderTimer
static int tolua_get_Core__HedgehogGame_m_renderTimer(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_renderTimer'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_renderTimer,"const Core::Time");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_timeScale of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_get_Core__HedgehogGame_m_timeScale
static int tolua_get_Core__HedgehogGame_m_timeScale(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_timeScale'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_timeScale);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_timeScale of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_set_Core__HedgehogGame_m_timeScale
static int tolua_set_Core__HedgehogGame_m_timeScale(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_timeScale'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_timeScale = ((double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_window of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_get_Core__HedgehogGame_m_window_ptr
static int tolua_get_Core__HedgehogGame_m_window_ptr(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_window'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->m_window,"Core::Window");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_window of class  Core::HedgehogGame */
#ifndef TOLUA_DISABLE_tolua_set_Core__HedgehogGame_m_window_ptr
static int tolua_set_Core__HedgehogGame_m_window_ptr(lua_State* tolua_S)
{
  Core::HedgehogGame* self = (Core::HedgehogGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_window'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Core::Window",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_window = ((Core::Window*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_core_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Core",0);
  tolua_beginmodule(tolua_S,"Core");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Color","Core::Color","",tolua_collect_Core__Color);
   #else
   tolua_cclass(tolua_S,"Color","Core::Color","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Color");
    tolua_variable(tolua_S,"r",tolua_get_Core__Color_r,tolua_set_Core__Color_r);
    tolua_variable(tolua_S,"g",tolua_get_Core__Color_g,tolua_set_Core__Color_g);
    tolua_variable(tolua_S,"b",tolua_get_Core__Color_b,tolua_set_Core__Color_b);
    tolua_variable(tolua_S,"a",tolua_get_Core__Color_a,tolua_set_Core__Color_a);
    tolua_function(tolua_S,"new",tolua_core_Core_Color_new00);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Color_new00_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Color_new00_local);
    tolua_function(tolua_S,"new",tolua_core_Core_Color_new01);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Color_new01_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Color_new01_local);
    tolua_function(tolua_S,"set",tolua_core_Core_Color_set00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Vec2","Core::Vec2","",tolua_collect_Core__Vec2);
   #else
   tolua_cclass(tolua_S,"Vec2","Core::Vec2","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Vec2");
    tolua_variable(tolua_S,"x",tolua_get_Core__Vec2_x,tolua_set_Core__Vec2_x);
    tolua_variable(tolua_S,"y",tolua_get_Core__Vec2_y,tolua_set_Core__Vec2_y);
    tolua_function(tolua_S,"new",tolua_core_Core_Vec2_new00);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Vec2_new00_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Vec2_new00_local);
    tolua_function(tolua_S,"new",tolua_core_Core_Vec2_new01);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Vec2_new01_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Vec2_new01_local);
    tolua_function(tolua_S,"set",tolua_core_Core_Vec2_set00);
    tolua_function(tolua_S,".add",tolua_core_Core_Vec2__add00);
    tolua_function(tolua_S,".sub",tolua_core_Core_Vec2__sub00);
    tolua_function(tolua_S,".sub",tolua_core_Core_Vec2__sub01);
    tolua_function(tolua_S,".mul",tolua_core_Core_Vec2__mul00);
    tolua_function(tolua_S,".div",tolua_core_Core_Vec2__div00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Random","Core::Random","",tolua_collect_Core__Random);
   #else
   tolua_cclass(tolua_S,"Random","Core::Random","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Random");
    tolua_function(tolua_S,"new",tolua_core_Core_Random_new00);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Random_new00_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Random_new00_local);
    tolua_function(tolua_S,"new",tolua_core_Core_Random_new01);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Random_new01_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Random_new01_local);
    tolua_function(tolua_S,"reseed",tolua_core_Core_Random_reseed00);
    tolua_function(tolua_S,"randInt",tolua_core_Core_Random_randInt00);
    tolua_function(tolua_S,"randFloat",tolua_core_Core_Random_randFloat00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Time","Core::Time","",tolua_collect_Core__Time);
   #else
   tolua_cclass(tolua_S,"Time","Core::Time","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Time");
    tolua_variable(tolua_S,"STOP_TIME",tolua_get_Core__Time_STOP_TIME,NULL);
    tolua_variable(tolua_S,"NORMAL_TIME",tolua_get_Core__Time_NORMAL_TIME,NULL);
    tolua_function(tolua_S,"microsFromSeconds",tolua_core_Core_Time_microsFromSeconds00);
    tolua_function(tolua_S,"microsFromMilis",tolua_core_Core_Time_microsFromMilis00);
    tolua_function(tolua_S,"new",tolua_core_Core_Time_new00);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Time_new00_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Time_new00_local);
    tolua_function(tolua_S,"update",tolua_core_Core_Time_update00);
    tolua_function(tolua_S,"updateBy",tolua_core_Core_Time_updateBy00);
    tolua_function(tolua_S,"getFixedStepUpdateCount",tolua_core_Core_Time_getFixedStepUpdateCount00);
    tolua_function(tolua_S,"getLastRealTimeMicros",tolua_core_Core_Time_getLastRealTimeMicros00);
    tolua_function(tolua_S,"getRealTimeMicros",tolua_core_Core_Time_getRealTimeMicros00);
    tolua_function(tolua_S,"getCurMicros",tolua_core_Core_Time_getCurMicros00);
    tolua_function(tolua_S,"getDeltaMicros",tolua_core_Core_Time_getDeltaMicros00);
    tolua_function(tolua_S,"getDeltaTime",tolua_core_Core_Time_getDeltaTime00);
    tolua_function(tolua_S,"getVirtCurMicros",tolua_core_Core_Time_getVirtCurMicros00);
    tolua_function(tolua_S,"getVirtDeltaMicros",tolua_core_Core_Time_getVirtDeltaMicros00);
    tolua_function(tolua_S,"getVirtDeltaTime",tolua_core_Core_Time_getVirtDeltaTime00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Transform","Core::Transform","",tolua_collect_Core__Transform);
   #else
   tolua_cclass(tolua_S,"Transform","Core::Transform","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Transform");
    tolua_function(tolua_S,"new",tolua_core_Core_Transform_new00);
    tolua_function(tolua_S,"new_local",tolua_core_Core_Transform_new00_local);
    tolua_function(tolua_S,".call",tolua_core_Core_Transform_new00_local);
    tolua_variable(tolua_S,"position",tolua_get_Core__Transform_position,tolua_set_Core__Transform_position);
    tolua_variable(tolua_S,"scale",tolua_get_Core__Transform_scale,tolua_set_Core__Transform_scale);
    tolua_variable(tolua_S,"rotation",tolua_get_Core__Transform_rotation,tolua_set_Core__Transform_rotation);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Core",0);
  tolua_beginmodule(tolua_S,"Core");
   tolua_constant(tolua_S,"WE_INVALID",Core::WE_INVALID);
   tolua_constant(tolua_S,"WE_KEYBOARDKEY",Core::WE_KEYBOARDKEY);
   tolua_constant(tolua_S,"WE_KEYBOARDTEXT",Core::WE_KEYBOARDTEXT);
   tolua_constant(tolua_S,"WE_MOUSEMOVE",Core::WE_MOUSEMOVE);
   tolua_constant(tolua_S,"WE_MOUSEBUTTON",Core::WE_MOUSEBUTTON);
   tolua_constant(tolua_S,"WE_MOUSEWHEEL",Core::WE_MOUSEWHEEL);
   tolua_constant(tolua_S,"WE_CLOSE",Core::WE_CLOSE);
   tolua_constant(tolua_S,"WE_LOSTFOCUS",Core::WE_LOSTFOCUS);
   tolua_constant(tolua_S,"WE_GAINFOCUS",Core::WE_GAINFOCUS);
   tolua_constant(tolua_S,"WE_FILECHANGE",Core::WE_FILECHANGE);
   tolua_constant(tolua_S,"WE_LASTEVENT",Core::WE_LASTEVENT);
   tolua_cclass(tolua_S,"KeyboardEvent","Core::KeyboardEvent","",NULL);
   tolua_beginmodule(tolua_S,"KeyboardEvent");
    tolua_variable(tolua_S,"m_keyCode",tolua_get_Core__KeyboardEvent_unsigned_m_keyCode,tolua_set_Core__KeyboardEvent_unsigned_m_keyCode);
    tolua_variable(tolua_S,"m_repeat",tolua_get_Core__KeyboardEvent_unsigned_m_repeat,tolua_set_Core__KeyboardEvent_unsigned_m_repeat);
    tolua_variable(tolua_S,"m_isDown",tolua_get_Core__KeyboardEvent_m_isDown,tolua_set_Core__KeyboardEvent_m_isDown);
    tolua_variable(tolua_S,"m_previouslyDown",tolua_get_Core__KeyboardEvent_m_previouslyDown,tolua_set_Core__KeyboardEvent_m_previouslyDown);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"MouseMoveEvent","Core::MouseMoveEvent","",NULL);
   tolua_beginmodule(tolua_S,"MouseMoveEvent");
    tolua_variable(tolua_S,"m_x",tolua_get_Core__MouseMoveEvent_m_x,tolua_set_Core__MouseMoveEvent_m_x);
    tolua_variable(tolua_S,"m_y",tolua_get_Core__MouseMoveEvent_m_y,tolua_set_Core__MouseMoveEvent_m_y);
    tolua_variable(tolua_S,"m_isRelative",tolua_get_Core__MouseMoveEvent_m_isRelative,tolua_set_Core__MouseMoveEvent_m_isRelative);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"MouseButtonEvent","Core::MouseButtonEvent","",NULL);
   tolua_beginmodule(tolua_S,"MouseButtonEvent");
    tolua_variable(tolua_S,"m_x",tolua_get_Core__MouseButtonEvent_m_x,tolua_set_Core__MouseButtonEvent_m_x);
    tolua_variable(tolua_S,"m_y",tolua_get_Core__MouseButtonEvent_m_y,tolua_set_Core__MouseButtonEvent_m_y);
    tolua_variable(tolua_S,"m_button",tolua_get_Core__MouseButtonEvent_unsigned_m_button,tolua_set_Core__MouseButtonEvent_unsigned_m_button);
    tolua_variable(tolua_S,"m_clicks",tolua_get_Core__MouseButtonEvent_unsigned_m_clicks,tolua_set_Core__MouseButtonEvent_unsigned_m_clicks);
    tolua_variable(tolua_S,"m_isDown",tolua_get_Core__MouseButtonEvent_m_isDown,tolua_set_Core__MouseButtonEvent_m_isDown);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"MouseWheelEvent","Core::MouseWheelEvent","",NULL);
   tolua_beginmodule(tolua_S,"MouseWheelEvent");
    tolua_variable(tolua_S,"m_x",tolua_get_Core__MouseWheelEvent_m_x,tolua_set_Core__MouseWheelEvent_m_x);
    tolua_variable(tolua_S,"m_y",tolua_get_Core__MouseWheelEvent_m_y,tolua_set_Core__MouseWheelEvent_m_y);
    tolua_variable(tolua_S,"m_scroll",tolua_get_Core__MouseWheelEvent_unsigned_m_scroll,tolua_set_Core__MouseWheelEvent_unsigned_m_scroll);
   tolua_endmodule(tolua_S);
   tolua_constant(tolua_S,"FILE_ADDED",Core::FILE_ADDED);
   tolua_constant(tolua_S,"FILE_MODIFIED",Core::FILE_MODIFIED);
   tolua_constant(tolua_S,"FILE_REMOVED",Core::FILE_REMOVED);
   tolua_constant(tolua_S,"FILE_RENAMED_FROM",Core::FILE_RENAMED_FROM);
   tolua_constant(tolua_S,"FILE_RENAMED_TO",Core::FILE_RENAMED_TO);
   tolua_constant(tolua_S,"FILE_BADDATA",Core::FILE_BADDATA);
   tolua_cclass(tolua_S,"FileChangeEvent","Core::FileChangeEvent","",NULL);
   tolua_beginmodule(tolua_S,"FileChangeEvent");
    tolua_variable(tolua_S,"m_index",tolua_get_Core__FileChangeEvent_unsigned_m_index,tolua_set_Core__FileChangeEvent_unsigned_m_index);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"WindowEvent","Core::WindowEvent","",NULL);
   tolua_beginmodule(tolua_S,"WindowEvent");
    tolua_variable(tolua_S,"m_type",tolua_get_Core__WindowEvent_unsigned_m_type,tolua_set_Core__WindowEvent_unsigned_m_type);
    tolua_variable(tolua_S,"m_timestamp",tolua_get_Core__WindowEvent_unsigned_m_timestamp,tolua_set_Core__WindowEvent_unsigned_m_timestamp);
    tolua_variable(tolua_S,"m_keyboard",tolua_get_Core__WindowEvent_m_keyboard,tolua_set_Core__WindowEvent_m_keyboard);
    tolua_variable(tolua_S,"m_mouseMove",tolua_get_Core__WindowEvent_m_mouseMove,tolua_set_Core__WindowEvent_m_mouseMove);
    tolua_variable(tolua_S,"m_mouseButton",tolua_get_Core__WindowEvent_m_mouseButton,tolua_set_Core__WindowEvent_m_mouseButton);
    tolua_variable(tolua_S,"m_mouseWheel",tolua_get_Core__WindowEvent_m_mouseWheel,tolua_set_Core__WindowEvent_m_mouseWheel);
    tolua_variable(tolua_S,"m_fileChange",tolua_get_Core__WindowEvent_m_fileChange,tolua_set_Core__WindowEvent_m_fileChange);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Window","Core::Window","",NULL);
   tolua_beginmodule(tolua_S,"Window");
    tolua_function(tolua_S,"showMessagebox",tolua_core_Core_Window_showMessagebox00);
    tolua_function(tolua_S,"resize",tolua_core_Core_Window_resize00);
    tolua_function(tolua_S,"move",tolua_core_Core_Window_move00);
    tolua_function(tolua_S,"showCursor",tolua_core_Core_Window_showCursor00);
    tolua_function(tolua_S,"setFullscreen",tolua_core_Core_Window_setFullscreen00);
    tolua_function(tolua_S,"setFileChangeDelay",tolua_core_Core_Window_setFileChangeDelay00);
    tolua_function(tolua_S,"getPositionX",tolua_core_Core_Window_getPositionX00);
    tolua_function(tolua_S,"getPositionY",tolua_core_Core_Window_getPositionY00);
    tolua_function(tolua_S,"getSizeX",tolua_core_Core_Window_getSizeX00);
    tolua_function(tolua_S,"getSizeY",tolua_core_Core_Window_getSizeY00);
    tolua_function(tolua_S,"isCursorShown",tolua_core_Core_Window_isCursorShown00);
    tolua_function(tolua_S,"isFullscreen",tolua_core_Core_Window_isFullscreen00);
    tolua_function(tolua_S,"getFileChangeDelay",tolua_core_Core_Window_getFileChangeDelay00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Core",0);
  tolua_beginmodule(tolua_S,"Core");
   tolua_cclass(tolua_S,"HedgehogGame","Core::HedgehogGame","",NULL);
   tolua_beginmodule(tolua_S,"HedgehogGame");
    tolua_variable(tolua_S,"m_logicTimer",tolua_get_Core__HedgehogGame_m_logicTimer,NULL);
    tolua_variable(tolua_S,"m_renderTimer",tolua_get_Core__HedgehogGame_m_renderTimer,NULL);
    tolua_variable(tolua_S,"m_timeScale",tolua_get_Core__HedgehogGame_m_timeScale,tolua_set_Core__HedgehogGame_m_timeScale);
    tolua_variable(tolua_S,"m_window",tolua_get_Core__HedgehogGame_m_window_ptr,tolua_set_Core__HedgehogGame_m_window_ptr);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_core (lua_State* tolua_S) {
 return tolua_core_open(tolua_S);
};
#endif

