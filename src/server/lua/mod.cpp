#include "mod.h"

#include "../server.h"

extern "C" {
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/luajit.h>
#include <luajit-2.1/lualib.h>
}

#include <iostream>

#include "spdlog.h"

using namespace std;

TypeManager_s* gTypemanager;

static int api_register_node(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    const char* texture_path[6] = {"!empty", "!empty", "!empty",
                                   "!empty", "!empty", "!empty"};
    if (!lua_istable(L, 2)) {
        lua_pushinteger(L, -1);
        return 1;
    }
    // lua_gettable(L, 2);
    int it = lua_gettop(L);
    lua_pushnil(L);
    int i = 0;
    while (lua_next(L, it)) {
        if (i > 5) {
            break;
        }
        texture_path[i] = lua_tostring(L, -1);
        lua_pop(L, 1);
        i++;
    }
    lua_pop(L, 1);

    gTypemanager->registerNode(name, texture_path);
    lua_pushnil(L);
    return 1;
}

static int api_register_item(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    const char* texture_path = luaL_checkstring(L, 2);
    lua_pushnil(L);
    gTypemanager->registerItem(name, texture_path);
    return 1;
}

int LuaErrorCallBack(lua_State* L)
{
    lua_Debug debug = {};
    //错误所处的的调用层级
    int rank = 0;
    //逐层获取lua抛出的错误，直到获取到
    while (lua_getstack(L, rank, &debug)) {
        rank++;
    }
    //判断栈顶是否为字符串类型
    int type = lua_type(L, -1);
    if (type != 4) {
        return 0;
    }
    std::string error = lua_tostring(L, -1);
    std::string errorShort = debug.short_src;
    int errorLine = debug.currentline;
    std::string errorMsg = "error:" + error + ",errorShort:" + errorShort +
                           ",line:" + std::to_string(errorLine);
    //将错误信息压人栈
    lua_pushstring(L, errorMsg.c_str());
    return 1;
}

bool Luaenv::init(TypeManager_s* tm)
{
    gTypemanager = tm;
    L = luaL_newstate();
    if (L == NULL) {
        return false;
    }
    luaL_openlibs(L);

    lua_register(L, "register_node", api_register_node);
    lua_register(L, "register_item", api_register_item);

    lua_pushcfunction(L, LuaErrorCallBack);

    return true;
}

bool Luaenv::exec(const char* path)
{
    int bRet = luaL_loadfile(L, path);
    if (bRet) {
        return false;
    }
    bRet = lua_pcall(L, 0, 0, lua_gettop(L));
    if (bRet != 0) {
        spdlog::error("{}", lua_tostring(L, -1));
        return false;
    }
    return true;
}

void Luaenv::destory()
{
    lua_close(L);
    return;
}

void Luaenv::execmods()
{
    exec("mods/default/init.lua");
}
