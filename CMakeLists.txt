# This file is maintained by Claude (https://claude.ai).
# Contents may be updated automatically during development sessions.

cmake_minimum_required(VERSION 3.20)
project(semnon_quest)

# generates compile_commands.json for clangd/IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# use C++23 (private project — 42's C++98/C++17 limit doesn't apply here)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# standard C++ only, no GNU dialect extensions (-std=c++23 instead of -std=gnu++23)
set(CMAKE_CXX_EXTENSIONS OFF)

# all source files — add new .cpp files here as the project grows
set(SOURCES
	src/main.cpp
	src/core/Game.cpp
	src/core/GameState.cpp
	src/core/Requirement.cpp
	src/core/Effect.cpp
	src/core/OptionType.cpp
	src/core/StateType.cpp
	src/ui/TerminalDisplay.cpp
	src/ui/Menu.cpp
	src/world/Location.cpp
	src/world/LocationManager.cpp
	src/world/Scene.cpp
	src/world/DialogueManager.cpp
	src/narrative/Dialogue.cpp
	src/utils/LocationLoader.cpp
	src/utils/ReactivityParser.cpp
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

# compiler warnings: the "strict" set widely used in real-world C++ projects
# (e.g. the C++ Best Practices guide), building on the 42 baseline -Wall -Wextra
#   -Wall                 despite the name not "all" warnings, but a curated set of near-certain mistakes:
#                         unused or uninitialized variables, a missing return in a non-void function,
#                         an enum value a switch without default doesn't handle, = instead of == in a
#                         condition, signed/unsigned comparisons, misleading indentation
#   -Wextra               a second set on top of -Wall: unused function parameters, comparisons that are
#                         always true/false (e.g. unsigned >= 0), struct fields left out of a brace
#                         initializer, a switch case falling through into the next one without break
#   -Wpedantic            anything the C++ standard itself doesn't allow, i.e. compiler extensions such as
#                         variable-length arrays (int a[n] with n only known at runtime); keeps the code
#                         portable to other compilers (pairs with CMAKE_CXX_EXTENSIONS OFF)
#   -Wshadow              a variable hides another variable or a member
#   -Wconversion          implicit conversions that can lose data (e.g. double -> int)
#   -Wsign-conversion     implicit signed <-> unsigned conversions (e.g. int -> size_t)
#   -Wold-style-cast      C-style casts like (int)x; use static_cast<int>(x)
#   -Wnon-virtual-dtor    a class with virtual functions but no virtual destructor
#   -Woverloaded-virtual  a derived-class function hides a base-class overload
target_compile_options(semnon_quest PRIVATE
	-Wall -Wextra -Wpedantic
	-Wshadow -Wconversion -Wsign-conversion -Wold-style-cast
	-Wnon-virtual-dtor -Woverloaded-virtual
)

# warnings as errors, like at 42. kept switchable because a different compiler version
# (e.g. on another machine) can add new warnings, which would otherwise break the build there.
# to disable: flip the default below to OFF (make re wipes build/, so a -D flag wouldn't stick)
option(SEMNON_WARNINGS_AS_ERRORS "treat compiler warnings as errors" ON)
if(SEMNON_WARNINGS_AS_ERRORS)
	target_compile_options(semnon_quest PRIVATE -Werror)
endif()

# sanitizers for debug builds (the Makefile always builds Debug), standard practice during development:
# AddressSanitizer catches memory errors (out-of-bounds access, use-after-free, leaks),
# UndefinedBehaviorSanitizer catches undefined behaviour (e.g. signed overflow) while the game runs.
# off by default because it needs extra packages: on fedora, sudo dnf install libasan libubsan
# afterwards flip the default below to ON
option(SEMNON_SANITIZERS "build Debug with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
if(SEMNON_SANITIZERS)
	target_compile_options(semnon_quest PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-omit-frame-pointer>")
	target_link_options(semnon_quest PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
endif()

# sets src/ as the include root so headers can be included without ../
# e.g. #include "core/GameState.hpp" instead of #include "../core/GameState.hpp"
# all subdirectories listed so headers can be included by filename alone anywhere in the project
target_include_directories(semnon_quest PRIVATE src src/core src/world src/ui src/utils src/narrative)

# expose the downloaded json.hpp to the project
# SYSTEM: warnings from inside this third-party header are suppressed (standard practice, they aren't ours to fix)
target_include_directories(semnon_quest SYSTEM PRIVATE ${CMAKE_BINARY_DIR}/include)
