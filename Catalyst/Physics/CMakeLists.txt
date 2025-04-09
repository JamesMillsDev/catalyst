cmake_minimum_required(VERSION 3.20)

project(catalyst.physics)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.physics SHARED ${SOURCE_FILES})

target_include_directories(catalyst.physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Maths/include
    ${reactphysics3d_SOURCE_DIR}
)

target_compile_definitions(catalyst.physics PRIVATE DLL_EXPORT)

target_link_libraries(catalyst.physics
    PRIVATE
        catalyst.core
        catalyst.maths
)

set_target_properties(catalyst.physics PROPERTIES FOLDER "Modules")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
