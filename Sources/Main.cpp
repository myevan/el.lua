#include "LuaMacros.h"
#include "ConsoleLogger.h"

#include "Character.h"
#include "World.h"

void OnTick(int index)
{
    printf("tick:%d\n", index);
}

int main()
{
    using namespace EL;

    ConsoleLogger logger;
    Logger::Set(&logger);

    World world;
    World::Set(&world);

    EL_LUA_CLASS_INTERFACE(Logger);
    EL_LUA_CLASS_FUNCTION(Logger, Get);
    EL_LUA_CLASS_METHOD(Logger, Info);
    EL_LUA_CLASS_END(Logger);
    EL_LUA_RETURN_TYPE(Logger);

    EL_LUA_CLASS_INTERFACE(Character);
    EL_LUA_CLASS_FUNCTION(Character, Get);
    EL_LUA_CLASS_METHOD(Character, DestroySelf);
    EL_LUA_CLASS_METHOD(Character, SetName);
    EL_LUA_CLASS_END(Character);
    EL_LUA_RETURN_TYPE(Character);

    EL_LUA_CLASS_INTERFACE(World);
    EL_LUA_CLASS_FUNCTION(World, Get);
    EL_LUA_CLASS_METHOD(World, CreateCharacter);
    EL_LUA_CLASS_END(World);
    EL_LUA_RETURN_TYPE(World);

    if (!EL_LUA_FILE_EXEC("Example01.lua"))
    {
        EL_LUA_ERROR_PRINT();
        return -1;
    }

    EL_LUA_FUNCTION_CALL("Event:Call", "OnTick", "tick");
    EL_LUA_FUNCTION_CALL("Test", "test");
    return 0;
}