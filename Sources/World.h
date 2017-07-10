#pragma once

#include "Singleton.h"

namespace EL
{
    class World : public Singleton<World>
    {
    public:
        int CreateCharacter(int key)
        {
            return 0;
        }
    };
}