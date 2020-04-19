#include <stdio.h>
#include <stdbool.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include"cJSON.h"

static int newjson(lua_State *L) {
    const char *jsonStr = luaL_checkstring(L, 1);
    cJSON *json = (cJSON *)lua_newuserdata(L, sizeof(cJSON));
    json->child = cJSON_Parse(jsonStr);
    luaL_getmetatable(L, "json");
    lua_setmetatable(L, -2);
    return 1;
}

static int getjson(lua_State *L) {
    cJSON *json  = (cJSON *)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);
    cJSON *jsonItem = cJSON_GetObjectItem( json->child , key );
    switch (jsonItem->type)
    {
        case cJSON_String: 
            lua_pushstring(L, jsonItem->valuestring); 
            break;
        case cJSON_Number:
            if (jsonItem->valuedouble != (double)jsonItem->valueint) {
                lua_pushnumber(L, jsonItem->valuedouble);
            }
            else {
                lua_pushinteger(L, jsonItem->valueint);
            }
            break;
        case cJSON_False:
            lua_pushboolean(L, false);
            break;
        case cJSON_True:
            lua_pushboolean(L, true);
            break; 
    }
    return 1;
}

static int setjson(lua_State *L) {
    cJSON *json  = (cJSON *)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);
    luaL_checkany(L, 3);
    cJSON *jsonItem = cJSON_GetObjectItem( json->child , key );
    if (lua_isboolean(L, 3))
    {
        bool flag = lua_toboolean(L, 3);
        if (flag) {
            jsonItem->type = cJSON_True;
        }
        else {
            jsonItem->type = cJSON_False;
        }
    }
    else if (lua_isinteger(L, 3))
    {
        int value = luaL_checkinteger(L, 3);
        jsonItem->type = cJSON_Number;
        jsonItem->valuedouble = (double)value;
        jsonItem->valueint = value;
    }
    else if (lua_isnumber(L, 3))
    {
        double value = luaL_checknumber(L, 3);
        jsonItem->type = cJSON_Number;
        jsonItem->valuedouble = value;
        jsonItem->valueint = (int)value;
    }
    else if (lua_isstring(L, 3))
    {
        const char *value = luaL_checkstring(L, 3);
        jsonItem->type = cJSON_String;
        cJSON_SetValuestring(jsonItem, value);
    }
    return 0;
}

static const struct luaL_Reg arraylib_f [] = {
    {"new", newjson},
    {NULL, NULL}
};

static const struct luaL_Reg arraylib_m [] = {
    {"__newindex", setjson},
    {"__index", getjson},
    {NULL, NULL}
};

int luaopen_json(lua_State *L)
{
    luaL_newmetatable(L, "json");
    luaL_setfuncs(L, arraylib_m, 0);
    luaL_newlib(L, arraylib_f);
    return 1;
}