#ifndef _MOD_H_
#define _MOD_H_
extern "C" {
#include <luajit-2.1/lua.h>
}

class Luaenv
{
public:
    bool init();
    bool exec(char* path);
    void destory();
    void execmods();

private:
    lua_State* L;
};
#endif