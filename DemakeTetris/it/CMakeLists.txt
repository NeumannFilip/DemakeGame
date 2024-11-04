# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(DemakeTetris)

# Set the C++ standard
set(CMAKE_PREFIX_PATH "C:/Users/mysla/Desktop/SDL2")
set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(SDL2 REQUIRED)


# Add the executable
add_executable(DemakeTetris it.cpp DemakeTetris.cpp)

# Include directories
target_include_directories(DemakeTetris PRIVATE ${SDL2_INCLUDE_DIRS})

# Link libraries
target_link_libraries(DemakeTetris PRIVATE ${SDL2_LIBRARIES} SDL2::SDL2 SDL2::SDL2main SDL2_ttf)