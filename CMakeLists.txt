cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Base
project(Project_Name VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_GENERATOR_PLATFORM x64)  # Say use x64
set(CMAKE_SUPPRESS_REGENERATION true)  #Anigilate ZERO

if (MSVC)
    add_compile_options(/MP)
endif()

add_subdirectory(Engine)  # Execute CMakeLists.txt
add_subdirectory(Editor)  # Execute CMakeLists.txt

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)  # Set as startup project to

message("//---------------------------------------------------------------------------------------------------")
message("--- Compiler id = ${CMAKE_CXX_COMPILER_ID} ---")
message("//---------------------------------------------------------------------------------------------------")
