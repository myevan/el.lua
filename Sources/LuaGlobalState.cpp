#include "LuaGlobalState.h"

extern "C" 
{
    #include "lualib.h"
};

namespace EL
{
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
    }

    LuaGlobalState::~LuaGlobalState()
    {
        lua_close(_state);
    }
}