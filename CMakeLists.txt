# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.12)

# Name your project and optionally set a version
project(Semnon_Quest VERSION 0.1)

# Require a specific C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add CONFIGURE_DEPENDS to your GLOB command to auto-watch for new files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Define the final executable name and the sources needed to build it
add_executable(semnon_quest ${SOURCES})

# Tell CMake where your header files live (Modern target-based approach)
target_include_directories(semnon_quest PRIVATE inc)

# Find SFML components
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# Link SFML to your project
target_link_libraries(semnon_quest sfml-graphics sfml-window sfml-system)
