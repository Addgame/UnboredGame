cmake_minimum_required(VERSION 3.15)
project(Unbored_Game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Unbored_Game_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/pugixml)
include_directories(./libs)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})

add_executable(Unbored_Game src/main.cpp src/screen.cpp src/player.cpp src/token.cpp src/scorecard.cpp src/game.cpp src/connections.cpp src/connections.h src/token.h src/player.h src/game.h src/screen.h src/application.h src/application.cpp src/texture.h src/texture.cpp src/font.cpp src/font.h src/parser.cpp src/parser.h src/selection_screen.cpp)
target_link_libraries(Unbored_Game ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})
target_link_libraries(Unbored_Game pugixml)
