#include "LuaMacros.h"
#include "ConsoleLogger.h"

int main()
{
    using namespace EL;

    ConsoleLogger logger;
    Logger::Set(&logger);

    EL_LUA_CLASS_INTERFACE(Logger);
    EL_LUA_CLASS_FUNCTION(Logger, Get);
    EL_LUA_CLASS_METHOD(Logger, Info);
    EL_LUA_CLASS_END(Logger);
    EL_LUA_RETURN_TYPE(Logger);

    if (!EL_LUA_FILE_EXEC("Example01.lua"))
    {
        EL_LUA_ERROR_PRINT();
        return -1;
    }
    return 0;
}