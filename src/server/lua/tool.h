#ifndef _LUA_TOOL_H_
#define _LUA_TOOL_H_
extern "C" {
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/luajit.h>
#include <luajit-2.1/lualib.h>
}
static void ReadTableFromIndex(
    lua_State* L, const char* lpszTableName, int index);
#endif