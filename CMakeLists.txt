cmake_minimum_required(VERSION 3.15)
project(Unbored_Game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Unbored_Game_SOURCE_DIR}/cmake")

add_subdirectory(libs/pugixml)
include_directories(./libs)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

add_executable(Unbored_Game src/main.cpp)
target_link_libraries(Unbored_Game ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})
target_link_libraries(Unbored_Game pugixml)