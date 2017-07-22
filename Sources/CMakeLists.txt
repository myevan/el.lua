set(FRAMEWORK_SOURCES
    Logger.cpp
    Buffer.cpp
    Event.cpp
    LuaState.cpp
    LuaGlobalState.cpp
    Framework.cpp)

add_executable(el_console Main.cpp ${FRAMEWORK_SOURCES})
target_link_libraries(el_console ${LUA_LIBRARIES})