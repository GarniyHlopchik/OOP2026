cmake_minimum_required(VERSION 3.15)
project(WinAPIProject CXX)

# Write compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

 # LAB6 exe ----------

set(LAB6_SOURCES
    main.cpp
    ui/window.cpp
    ui/dialogue_window.cpp
    ui/lab6_win.cpp
    helpers.cpp
    ui/dialogue.cpp
)

# Create the executable target
add_executable(lab6 WIN32 ${LAB6_SOURCES})

# Add UNICODE and _UNICODE definitions
target_compile_definitions(lab6 PRIVATE UNICODE _UNICODE)

target_include_directories(lab6 PRIVATE ui)

# Link the required Windows API libraries
target_link_libraries(lab6 PRIVATE gdi32 comctl32 comdlg32)

# OBJECT2 exe ------------

set(OBJECT2_SOURCES
    obj2.cpp
    ui/window.cpp
    ui/dialogue_window.cpp
    ui/obj2_win.cpp
)
# Create the executable target
add_executable(obj2 WIN32 ${OBJECT2_SOURCES})

# Add UNICODE and _UNICODE definitions
target_compile_definitions(obj2 PRIVATE UNICODE _UNICODE)

target_include_directories(obj2 PRIVATE ui)

# Link the required Windows API libraries
target_link_libraries(obj2 PRIVATE gdi32 comctl32 comdlg32)

# OBJECT3 exe ------------

set(OBJECT2_SOURCES
    obj3.cpp
    ui/window.cpp
    ui/dialogue_window.cpp
    obj3/obj3_win.cpp
)
# Create the executable target
add_executable(obj3 WIN32 ${OBJECT2_SOURCES})

# Add UNICODE and _UNICODE definitions
target_compile_definitions(obj3 PRIVATE UNICODE _UNICODE)

target_include_directories(obj3 PRIVATE ui)

# Link the required Windows API libraries
target_link_libraries(obj3 PRIVATE gdi32 comctl32 comdlg32)

if(MSVC)
    target_compile_options(lab6 PRIVATE /utf-8)
    target_compile_options(obj2 PRIVATE /utf-8)
    target_compile_options(obj3 PRIVATE /utf-8)
endif()
