# Engine

set(ENGINE_SOURCES  # All .cpp store here
    Source/Core/Core.cpp
)

add_library(Engine STATIC ${ENGINE_SOURCES})  # Add static library

target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/Engine/Include)  # where we store includes
