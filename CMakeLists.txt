cmake_minimum_required(VERSION 3.15)
project(Unbored_Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Unbored_Game_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

include_directories("C:/MinGW/include")

set(CMAKE_CXX_STANDARD 17)

add_executable(Unbored_Game main.cpp)
target_link_libraries(Unbored_Game ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})

target_link_libraries(Unbored_Game "C:/MinGW/lib/pugixml.cpp")