cmake_minimum_required(VERSION 3.20)

project(catalyst.input)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.input SHARED ${SOURCE_FILES})

target_include_directories(catalyst.input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Maths/include
    ${imgui_SOURCE_DIR}
)

target_compile_definitions(catalyst.input PRIVATE DLL_EXPORT)

target_link_libraries(catalyst.input
    PRIVATE
        catalyst.core
        catalyst.maths
        glfw
	imgui
)

set_target_properties(catalyst.input PROPERTIES 
	FOLDER "Modules"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
