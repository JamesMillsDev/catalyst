cmake_minimum_required(VERSION 3.20)

project(SampleGame)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_executable(SampleGame ${SOURCE_FILES})

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

target_include_directories(SampleGame
    PUBLIC include
    PRIVATE source
)

target_link_libraries(SampleGame
    PRIVATE
        catalyst.core
        catalyst.graphics
        catalyst.input
        catalyst.maths
        catalyst.scripting
        catalyst.gameplay
        catalyst.physics
        glfw
        glad
        imgui
)

set_target_properties(SampleGame PROPERTIES FOLDER "Applications")
