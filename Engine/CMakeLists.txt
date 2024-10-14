# Engine

set(ENGINE_SOURCES Source/Core/Core.cpp)  # All .cpp store here
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/Engine/Plugins/lib)  # Set DIRECTIONS at LIBRARY_DIR

add_library(Engine STATIC ${ENGINE_SOURCES})  # Add static library

target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/Engine/Include)  # where we store includes | Main
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/Engine/Plugins/Includes)  # where we store includes | Main

target_link_directories(Engine PUBLIC ${LIBRARY_DIR})
target_link_libraries(Engine PRIVATE lib_crt.lib)  # lib_crt.lib else_crt.lib
