# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.8)

set(32BLIT_PATH "../" CACHE PATH "Path to 32blit.cmake")
set(PROJECT_NAME COop)
set(PROJECT_SOURCE game.cpp game.hpp splash.cpp splash.hpp play.cpp play.hpp menu.cpp menu.hpp scoretable.cpp scoretable.hpp endgame.cpp endgame.hpp)

# Build configuration; approach this with caution!
project (${PROJECT_NAME})
include (${32BLIT_PATH}/32blit.cmake)
blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
