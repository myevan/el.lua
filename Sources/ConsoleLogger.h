#pragma once

#include "Logger.h"

namespace EL
{
    class ConsoleLogger : public Logger
    {
    public:
        virtual void Info(const char* text) override
        {
            puts(text);
        }
    };
}
