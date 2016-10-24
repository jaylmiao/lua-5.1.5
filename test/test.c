/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/07/16 16:51:38 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jaymiao (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
 
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int my_panic(lua_State *L)
{
    (void)L;
    fprintf(stderr, "this is in my_panic\n");
    return 0;
}

static void stackDump(lua_State *L){
    int i;
    int top = lua_gettop(L);
    printf("addr:%p, total element:%d\t", L, top);
    for(i = 1; i <= top; i++)
    {
        int t = lua_type(L, i);
        switch(t)
        {
            case LUA_TSTRING:
                printf("'%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf(lua_toboolean(L, i) ? "true":"false");
                break;
            case LUA_TNUMBER:
                printf("%g", lua_tonumber(L, i));
                break;
            default:
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("  ");
    }
    printf("\n");
}

int luaadd(lua_State *L, int a, int b)
{
    int sum = 0;
    lua_getglobal(L, "add");
    lua_pushnumber(L, a);
    lua_pushnumber(L, b);
    lua_call(L, 2, 1);
    sum = (int) lua_tonumber(L, -1);
    lua_pop(L, -1);
    return sum;
}

int main()
{
    lua_State *L = lua_open();
    if(!L)
    {
        printf("create lua_State failed\n");
        return -1;
    }

    int stack_size = lua_checkstack(L, 10);
    printf("stack_size check:%d\n", stack_size);

    lua_pushstring(L, "hello");
    lua_pushstring(L, "jaymiao");
    lua_concat(L, 2);
    lua_createtable(L, 1, 2);
    stackDump(L);
    lua_atpanic(L, my_panic);
    //lua_error(L);

    lua_State *L1 = lua_newthread(L);
    if(!L1)
    {
        printf("create new thread failed\n");
        return -1;
    }
    
    const char *filename = "test.lua";
    if(luaL_loadfile(L1, filename) || lua_pcall(L1, 0, 0, 0))
    {
        printf("Error Msg is %s.\n",lua_tostring(L1 ,-1));
        return -1;
    }
    int sum = luaadd(L1, 10, 15);
    printf("sum:%d\n", sum);

    lua_getglobal(L1, "Func1");
    lua_getglobal(L1, "test");
    lua_pushstring(L1, "for test");
    //lua_setfield(L1, -2, "a");
    lua_pushstring(L1, "a");
    lua_gettable(L1, -3);
    //lua_getfield(L1, -1, "a");
    lua_pushinteger(L1, 10);

    stackDump(L);
    stackDump(L1);

    int bRet = lua_resume(L1, 1);
    printf("resume: %d\n", bRet);
    //stackDump(L1);

    lua_close(L);

    return 0;
}
