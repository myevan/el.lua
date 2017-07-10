#pragma once

#include "Singleton.h"

namespace EL
{
    class Logger : public Singleton<Logger>
    {
    public:
        virtual ~Logger() = default;
        virtual void Info(const char* text) = 0;

    protected:
        Logger() = default;
    };

}
