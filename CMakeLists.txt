cmake_minimum_required(VERSION 3.12)
project(Luminox LANGUAGES C CXX)

# build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# executable
add_executable(main src/main.cpp)

# compile settings
target_compile_features(main PRIVATE cxx_std_11)
set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF)

# src
target_sources(main PRIVATE src/main.cpp)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
endif()