#pragma once

#include "LuaGlobalState.h"

#define EL_LUA_FUNCTION(func) \
    EL::LuaGlobalState::Get().BindLuaFunction(#func, [](lua_State* state) { \
        return EL::LuaState(state).InvokeCFunction(func); })

#define EL_LUA_CLASS(cls) \
    EL::LuaGlobalState::Get().BeginLuaClass( \
        #cls, \
        [](lua_State* state) { \
            return EL::LuaState(state).CreateCppInstance<cls>(); }, \
        [](lua_State* state) { \
            return EL::LuaState(state).DestroyCppInstance<cls>(); }) 

#define EL_LUA_CLASS_INTERFACE(cls) \
    EL::LuaGlobalState::Get().BeginLuaClassInterface(#cls)

#define EL_LUA_CLASS_FUNCTION(cls, method) \
    EL::LuaGlobalState::Get().BindLuaClassMethod(#method, [](lua_State* state) { \
        return EL::LuaState(state).InvokeCppStaticMethod(&cls::method); })

#define EL_LUA_CLASS_METHOD(cls, method) \
    EL::LuaGlobalState::Get().BindLuaClassMethod(#method, [](lua_State* state) { \
        return EL::LuaState(state).InvokeCppBoundMethod(&cls::method); })

#define EL_LUA_CLASS_END(cls) EL::LuaGlobalState::Get().EndLuaClass(#cls)

#define EL_LUA_RETURN_TYPE(t) EL::LuaGlobalState::Get().RegisterLuaReturnType<t>(#t)

#define EL_LUA_FILE_EXEC(...) EL::LuaGlobalState::Get().ExecLuaFile(__VA_ARGS__)

#define EL_LUA_ERROR_PRINT(...) EL::LuaGlobalState::Get().PrintError(__VA_ARGS__)
