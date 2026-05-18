# This file is maintained by Claude (https://claude.ai).
# Contents may be updated automatically during development sessions.

cmake_minimum_required(VERSION 3.16)
project(semnon_quest)

# generates compile_commands.json for clangd/IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
	src/utils/LocationLoader.cpp
)

# fetch nlohmann/json at configure time — no manual install required
include(FetchContent)
FetchContent_Declare(
	nlohmann_json
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
	DOWNLOAD_NO_EXTRACT TRUE
	DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/include/nlohmann
)
FetchContent_MakeAvailable(nlohmann_json)

# make executable appear on root level
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(semnon_quest ${SOURCES})

# sets src/ as the include root so headers can be included without ../
# e.g. #include "core/GameState.hpp" instead of #include "../core/GameState.hpp"
# all subdirectories listed so headers can be included by filename alone anywhere in the project
target_include_directories(semnon_quest PRIVATE src src/core src/world src/ui src/utils)

# expose the downloaded json.hpp to the project
target_include_directories(semnon_quest PRIVATE ${CMAKE_BINARY_DIR}/include)
