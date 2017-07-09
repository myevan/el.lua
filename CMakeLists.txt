cmake_minimum_required(VERSION 3.5)

project(cpplua)

set(CMAKE_CXX_STANDARD 11)

find_package(Lua REQUIRED)

include_directories("${LUA_INCLUDE_DIR}")

add_subdirectory(Sources)