#include "LuaState.h"

namespace EL
{
    LuaState::LuaState(lua_State* state)
    {
        _state = state;
    }

    LuaState::~LuaState()
    {
    }

    bool LuaState::IsValid()
    {
        return _state != nullptr;
    }

    void LuaState::BindLuaFunction(const char* name, FunctionType func)
    {
        lua_pushcfunction(_state, func);
        lua_setglobal(_state, name);
    }

    void LuaState::BeginLuaClass(const char* name, FunctionType create, FunctionType destroy)
    {
        // local _0 = { __name="name" }
        luaL_newmetatable(_state, name);
        
        // setmetatable(_0, _0)
        lua_pushvalue(_state, -1);
        lua_setmetatable(_state, -1);

        // _0.__index = _0
        lua_pushliteral(_state, "__index");
        lua_pushvalue(_state, -2);
        lua_rawset(_state, -3); 

        // _0.__call = function(cls)
        lua_pushliteral(_state, "__call");
        lua_pushcfunction(_state, create);
        lua_rawset(_state, -3);

        // _0.__gc = function(cls)
        lua_pushliteral(_state, "__gc");
        lua_pushcfunction(_state, destroy);
        lua_rawset(_state, -3);
    }

    void LuaState::BeginLuaClassInterface(const char* name)
    {
        // local _0 = { __name="name" }
        luaL_newmetatable(_state, name);
        
        // setmetatable(_0, _0)
        lua_pushvalue(_state, -1);
        lua_setmetatable(_state, -1);

        // _0.__index = _0
        lua_pushliteral(_state, "__index");
        lua_pushvalue(_state, -2);
        lua_rawset(_state, -3); 
    }

    void LuaState::BindLuaClassMethod(const char* name, FunctionType func)
    {
        lua_pushstring(_state, name);
        lua_pushcfunction(_state, func);
        lua_rawset(_state, -3);
    }

    void LuaState::EndLuaClass(const char* name)
    {
        lua_setglobal(_state, name);
    }

    bool LuaState::ExecLuaFile(const char* filePath)
    {
        if (luaL_dofile(_state, filePath) != LUA_OK)
        {
            return false;
        }

        return true;
    }

    void LuaState::PrintError()
    {
        puts(PeekLastString());
    }

    const char* LuaState::PeekLastString()
    {
        return lua_tostring(_state, -1);
    }

    int LuaState::Push(int(*input)(lua_State*))
    {
        lua_pushcfunction(_state, input);
        return 1;
    }

    int LuaState::Push(int input)
    {
        lua_pushinteger(_state, input);
        return 1;
    }

    int LuaState::Push(const char** inputs)
    {
        lua_pushlightuserdata(_state, inputs);
        return 1;
    }
}
