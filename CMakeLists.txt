
# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Project name and C++ standard
project(CommediaAnalyzer)
set(CMAKE_CXX_STANDARD 17)

# Find Poppler using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER REQUIRED poppler)

# Include directories and link libraries
include_directories(${POPPLER_INCLUDE_DIRS})
link_directories(${POPPLER_LIBRARY_DIRS})

# Add the executable
add_executable(CommediaAnalyzer SingleAnalPDF.cpp)

# Link Poppler libraries to the target
target_link_libraries(CommediaAnalyzer ${POPPLER_LIBRARIES})
