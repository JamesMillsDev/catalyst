cmake_minimum_required(VERSION 3.20)

project(catalyst.graphics)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.graphics SHARED ${SOURCE_FILES})

target_include_directories(catalyst.graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Maths/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Gameplay/include
    ${glad_SOURCE_DIR}/generated
    ${assimp_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}
    ${stb_SOURCE_DIR}
)

target_compile_definitions(catalyst.graphics PRIVATE DLL_EXPORT GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(catalyst.graphics
    PRIVATE
        catalyst.core
        catalyst.maths
        catalyst.gameplay
        glad
        glfw
        assimp
)

set_target_properties(catalyst.graphics PROPERTIES FOLDER "Modules")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
