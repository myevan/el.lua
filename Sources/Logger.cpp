#include "Logger.h"

#include <assert.h>

namespace EL
{
    Logger* Logger::_inst = nullptr;

    Logger& Logger::Get()
    {
        assert(_inst != nullptr);
        return *_inst;
    }

    void Logger::Set(Logger* inst)
    {
        assert(inst != nullptr);
        _inst = inst;
    }
}
