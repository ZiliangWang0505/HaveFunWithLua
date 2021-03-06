#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "limits.h"

#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))
#define checkarrary(L) (BitArray *)luaL_checkudata(L, 1, "LuaBook.array")

typedef struct BitArray
{
    int size;
    unsigned int values[1];
} BitArray;

static int newarray(lua_State *L)
{
    int i;
    size_t nbytes;
    BitArray *a;

    int n = (int)luaL_checkinteger(L, 1);
    luaL_argcheck(L, n >=1, 1, "invalid size");
    nbytes = sizeof(BitArray) + I_WORD(n-1)*sizeof(unsigned int);
    a = (BitArray *)lua_newuserdata(L, nbytes);
    a->size = n;
    for(i=0; i <= I_WORD(n-1); i++)
    {
        a->values[i] = 0;
    }
    luaL_getmetatable(L, "LuaBook.array");
    lua_setmetatable(L, -2);
    return 1;
}

static unsigned int *getparams(lua_State *L, unsigned int *mask) {
    BitArray *a = checkarrary(L);
    int index = (int)luaL_checkinteger(L, 2) - 1;
    luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");
    *mask = I_BIT(index);
    return &a->values[I_WORD(index)];
} 

static int setarray(lua_State *L)
{
    unsigned int mask;
    unsigned int *entry = getparams(L, &mask);
    luaL_checkany(L, 3);
    if(lua_toboolean(L, 3))
    {
        *entry |= mask;
    }
    else
    {
        *entry &= ~mask;
    }
    return 0;
}

static int getarray(lua_State *L)
{
    unsigned int mask;
    unsigned int *entry = getparams(L, &mask);
    lua_pushboolean(L, *entry & mask);
    return 1;
}

static int getsize(lua_State *L)
{
    BitArray *a = checkarrary(L);
    lua_pushinteger(L, a->size);
    return 1;
}

int array2string(lua_State *L) {
    BitArray *a = checkarrary(L);
    lua_pushfstring(L, "array(%d)", a->size);
    return 1;
}

static const struct luaL_Reg arraylib_f [] = {
    {"new", newarray},
    {NULL, NULL}
};

static const struct luaL_Reg arraylib_m [] = {
    {"__newindex", setarray},
    {"__index", getarray},
    {"__len", getsize},
    {"__tostring", array2string},
    {NULL, NULL}
};

int luaopen_array(lua_State *L)
{
    luaL_newmetatable(L, "LuaBook.array");
    luaL_setfuncs(L, arraylib_m, 0);
    luaL_newlib(L, arraylib_f);
    return 1;
}