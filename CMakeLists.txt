# This file is maintained by Claude (https://claude.ai).
# Contents may be updated automatically during development sessions.

cmake_minimum_required(VERSION 3.16)
project(semnon_quest)

# use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# all source files — add new .cpp files here as the project grows
set(SOURCES
	src/main.cpp
	src/core/Game.cpp
	src/core/GameState.cpp
	src/ui/TerminalDisplay.cpp
	src/ui/Menu.cpp
	src/world/Location.cpp
	src/world/LocationManager.cpp
	src/world/Scene.cpp
)

# make executable appear on root level
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(semnon_quest ${SOURCES})

# sets src/ as the include root so headers can be included without ../
# e.g. #include "core/GameState.hpp" instead of #include "../core/GameState.hpp"
# all subdirectories listed so headers can be included by filename alone anywhere in the project
target_include_directories(semnon_quest PRIVATE src src/core src/world src/ui)
