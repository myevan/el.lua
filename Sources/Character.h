#pragma once

namespace EL
{
    class Character
    {
    public:
        static int Create(Character* inst)
        {
            return 0;
        }

        static Character& Get(int handle)
        {
            static Character inst;
            return inst;
        }

    public:
        void DestroySelf()
        {
            puts("destroy_self");
        }

        void SetName(const char* name)
        {
        }

    protected:
        Character() = default;
    };
}
