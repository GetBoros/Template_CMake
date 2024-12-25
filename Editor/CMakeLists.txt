# Editor

set(CMAKE_WIN32_EXECUTABLE TRUE)  # Use WinMain

include_directories("Include")  # find includes here

add_executable(Editor
        Include/Window.cpp
        Include/Window.h
		Include/Main.cpp  # start point
		Include/Main.h )

#Make Folders
source_group("Main" FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Main.h
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Main.cpp )

source_group("Window" FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Window.h
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Window.cpp )

#  Добавляем зависимость от библиотеки движка
target_link_libraries(Editor PRIVATE Engine)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/Engine/Include)  # From CMake Source direction to engine includes

if (WIN32)
    target_sources(Editor PRIVATE Include/WindowsProject1.rc)  # if win32 app use resources
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)  # Set as startup project to

# message("Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")  # cmake .. -G "Visual Studio 17 2022"