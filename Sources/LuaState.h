#pragma once

extern "C" 
{
    #include "lua.h"
    #include "lauxlib.h"
};

#include <functional>

namespace EL
{
    typedef int(*FunctionType)(lua_State*);

    template<typename T>
    struct LuaReturnType
    {
        static const char* name;
    };

    template<typename T>
    const char* LuaReturnType<T>::name = nullptr;

    class LuaState
    {
    public:
        LuaState(lua_State* state);

    public:
        virtual ~LuaState();

    public:
        bool IsValid();

    public:
        void BindLuaFunction(const char* name, FunctionType func);

    public:
        void BeginLuaClass(const char* name, FunctionType func);
        void BeginLuaClassInterface(const char* name);
        void BindLuaClassMethod(const char* name, FunctionType func);
        void EndLuaClass(const char* name);

    public:
        bool ExecLuaFile(const char* filePath);

    public:
        void PrintError();

    private:
        const char* PeekLastString();

    private:
        int Push(int(*input)(lua_State*));
        int Push(int input);
        int Push(const char** inputs);

    public:
        template<typename Type>
        void RegisterLuaReturnType(const char* name)
        {
            LuaReturnType<Type>::name = name;
        }

        template<typename ClsType>
        int CreateCppInstance() 
        {
            // _1 = {userdata}
            ClsType** userData = static_cast<ClsType**>(lua_newuserdata(_state, sizeof(ClsType*)));
            *userData = new ClsType();

            // setmetatable(_1, cls)
            lua_pushvalue(_state, 1);
            lua_setmetatable(_state, -2);

            return 1; 
        }

    public:
        template<typename ... ArgTypes>
        void CallLuaFunction(FunctionType func, ArgTypes ... args)
        {
            Push(func);
            int argCount = Push(args...);
            int retCount = 0;
            if (lua_pcall(_state, argCount, retCount, 0) != LUA_OK) 
            {
                // TODO: 
                printf("LUA_PCALL_ERROR:%s\n", PeekLastString());
            }
        }

    public:
        template<typename RetType, typename ... ArgTypes>
        int InvokeCFunction(RetType (*func)(ArgTypes...)) 
        {
            int index = 1;
            Push(func(Check<ArgTypes>(&index)...));
            return 1;
        }

        template<typename ... ArgTypes>
        int InvokeCFunction(void (*func)(ArgTypes...)) 
        {
            int index = 1;
            func(Check<ArgTypes>(&index)...);
            return 0;
        }

    public:
        template<typename ClsType, typename RetType, typename ... ArgTypes>
        int InvokeCppMethod(RetType (ClsType::*method)(ArgTypes...)) 
        {
            ClsType** instRef = static_cast<ClsType**>(lua_touserdata(_state, 1));
            if (instRef == nullptr)
            {
                // TODO: error
                return 0;
            }

            int index = 2;
            Push((*instRef->*method)(Check<ArgTypes>(&index)...));
            return 1;
        }

        template<typename ClsType, typename ... ArgTypes>
        int InvokeCppMethod(void (ClsType::*method)(ArgTypes...)) 
        {
            ClsType** instRef = static_cast<ClsType**>(lua_touserdata(_state, 1));
            if (instRef == nullptr)
            {
                // TODO: error
                return 0;
            }

            int index = 2;
            (*instRef->*method)(Check<ArgTypes>(&index)...);
            return 0;
        }

    private:
        template<typename HeadType, typename ... TailTypes>
        int Push(HeadType head, TailTypes ... tails)
        {
            return Push(head) + Push(tails...);
        }

    private:
        template<typename ClsType>
        int Push(ClsType* input)
        {
            // _0 = registry[input]
            lua_pushlightuserdata(_state, input);

            const char* typeName = LuaReturnType<ClsType>::name;
            if (typeName != nullptr)
            {
                lua_gettable(_state, LUA_REGISTRYINDEX);

                if (lua_isnil(_state, -1))
                {
                    // _1 = {userdata}
                    ClsType** inputRef = static_cast<ClsType**>(lua_newuserdata(_state, sizeof(ClsType*)));
                    *inputRef = input;

                    // setmetatable(_1, getmetatable(cls))
                    luaL_getmetatable(_state, typeName);
                    lua_setmetatable(_state, -2);

                    // registry[input] = _1
                    lua_pushlightuserdata(_state, input);
                    lua_pushvalue(_state, -2);
                    lua_settable(_state, LUA_REGISTRYINDEX);
                }
            }
            return 1;
        }

        template<typename ClsType>
        int Push(ClsType& input)
        {
            return Push(&input);
        }

    private:
        template<typename RetType>
        RetType Check(int* indexPtr)
        {
            return CheckWrapper<RetType>::Invoke(_state, (*indexPtr)++);
        }

    private:
        template<typename RetType, typename=void>
        struct CheckWrapper
        {
            static void Invoke(lua_State* state, int index)
            {
            }
        };

        template<typename Dummy>
        struct CheckWrapper<int, Dummy>
        {
            static int Invoke(lua_State* state, int index)
            {
                return luaL_checknumber(state, index);
            }
        };

        template<typename Dummy>
        struct CheckWrapper<const char*, Dummy>
        {
            static const char* Invoke(lua_State* state, int index)
            {
                return luaL_checkstring(state, index);
            }
        };

    protected:
        lua_State* _state;
    };

}
