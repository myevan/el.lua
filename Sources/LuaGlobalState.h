#pragma once

#include "LuaState.h"

namespace EL
{
    class LuaGlobalState : public LuaState
    {
    public:
        static LuaGlobalState& Get();

    private:
        LuaGlobalState();

    private:
        virtual ~LuaGlobalState();
    };
}