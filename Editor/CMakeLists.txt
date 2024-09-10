# Editor

set(CMAKE_WIN32_EXECUTABLE TRUE)  # Use WinMain

include_directories("Include")  # find includes here

add_executable(Editor Include/Main.cpp )  # start point

target_link_libraries(Editor PRIVATE Engine)  # Добавляем зависимость от библиотеки движка
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/Engine/Include)  # From CMake Source direction to engine includes

if (WIN32)
    target_sources(Editor PRIVATE Include/WindowsProject1.rc)  # if win32 app use resources
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)  # Set as startup project to
