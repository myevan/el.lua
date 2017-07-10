#pragma once

#include <assert.h>

namespace EL
{
    template<typename ClsType>
    class Singleton
    {
    public:
        static ClsType& Get()
        {
            assert(_inst != nullptr);
            return *_inst;
        }

        static void Set(ClsType* inst)
        {
            assert(inst != nullptr);
            _inst = inst;
        }

    private:
        static ClsType* _inst;
    };

    template<typename ClsType>
    ClsType* Singleton<ClsType>::_inst = nullptr;
}