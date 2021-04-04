#ifndef _MOD_H_
#define _MOD_H_
extern "C" {
#include <luajit-2.1/lua.h>
}

#include "../block_s.h"

class Luaenv
{
public:
    bool init(TypeManager_s* tm);
    bool exec(char* path);
    void destory();
    void execmods();

private:
    lua_State* L;
};
#endif