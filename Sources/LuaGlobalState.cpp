#include "LuaGlobalState.h"

extern "C" 
{
    #include "lualib.h"
};

namespace EL
{
    int Derived(lua_State* state)
    {
        // _0 = {_cls, params}[2]
        if (!lua_checkstack(state, 2))
        {
            puts("WRONG ARGUMENTS");
            return 0;
        }

        lua_pushvalue(state, 2);

        // setmetatable(_0, cls)
        lua_pushvalue(state, 1); lua_setmetatable(state, -2);

        // _0.__index = cls
        lua_pushliteral(state, "__index");
        lua_pushvalue(state, 2);
        lua_rawset(state, -3); 

        // _0.__newindex = cls
        lua_pushliteral(state, "__newindex");
        lua_pushvalue(state, 2);
        lua_rawset(state, -3); 

        return 1;
    }

    LuaGlobalState& LuaGlobalState::Get()
    {
        static LuaGlobalState s_inst;
        return s_inst;
    }

    LuaGlobalState::LuaGlobalState()
    : LuaState(luaL_newstate())
    {
        luaL_openlibs(_state);
        luaopen_base(_state);

        lua_pushcfunction(_state, Derived);
        lua_setglobal(_state, "derived");
    }

    LuaGlobalState::~LuaGlobalState()
    {
        lua_close(_state);
    }
}