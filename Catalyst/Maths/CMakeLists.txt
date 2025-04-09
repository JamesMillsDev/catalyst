cmake_minimum_required(VERSION 3.20)

project(catalyst.maths)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.maths SHARED ${SOURCE_FILES})

target_include_directories(catalyst.maths PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${glm_SOURCE_DIR}
)

target_compile_definitions(catalyst.maths PRIVATE DLL_EXPORT GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(catalyst.maths PRIVATE
    catalyst.core
)

set_target_properties(catalyst.maths PROPERTIES FOLDER "Modules")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
