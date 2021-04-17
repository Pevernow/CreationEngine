#include "tool.h"

static void ReadTableFromIndex(
    lua_State* L, const char* lpszTableName, int index)
{
    lua_gettable(L, index);
    int it = lua_gettop(L);
    lua_pushnil(L);
    while (lua_next(L, it)) {
        lua_tostring(L, -1) lua_pop(L, 1);
    }
}