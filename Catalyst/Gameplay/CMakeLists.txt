cmake_minimum_required(VERSION 3.20)

project(catalyst.gameplay)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.gameplay SHARED ${SOURCE_FILES})

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

target_include_directories(catalyst.gameplay PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Maths/include
)

target_compile_definitions(catalyst.gameplay PRIVATE DLL_EXPORT)

target_link_libraries(catalyst.gameplay
    PRIVATE
        catalyst.core
        catalyst.maths
        glad
        glfw
)

set_target_properties(catalyst.gameplay PROPERTIES FOLDER "Modules")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
