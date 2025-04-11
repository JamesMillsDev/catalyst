cmake_minimum_required(VERSION 3.20)

project(catalyst.core)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_library(catalyst.core SHARED ${SOURCE_FILES})
message(STATUS "✅ Core module: added target catalyst.core")

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

# Set include directories
target_include_directories(catalyst.core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glad_SOURCE_DIR}/generated
    ${glfw_SOURCE_DIR}
    ${glm_SOURCE_DIR}
    ${pugixml_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(catalyst.core PUBLIC
    glad
    glfw
    pugixml
)

# Set preprocessor definition for DLL export
target_compile_definitions(catalyst.core PRIVATE DLL_EXPORT)

# Set Visual Studio folder
target_sources(catalyst.core PRIVATE ${CORE_SRC})
set_target_properties(catalyst.core PROPERTIES FOLDER "Modules")

# Ensure headers are visible in VS
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${CORE_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Define paths
set(INPUT_XML "${CMAKE_CURRENT_SOURCE_DIR}/config/engine.xml")  # Change to your file
set(OUTPUT_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/EngineConfig.h")  # Store in include folder
set(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}/CMake/embed_file.py")

add_custom_target(EmbedEngineConfig
    COMMAND ${Python_EXECUTABLE} ${PYTHON_SCRIPT} ${INPUT_XML} ${OUTPUT_HEADER}
    COMMENT "Embedding file: ${INPUT_XML} -> ${OUTPUT_HEADER}"
    VERBATIM
)

add_dependencies(catalyst.core EmbedEngineConfig)

get_property(_IS_GLOBAL TARGET catalyst.core PROPERTY NAME)
message(STATUS "🧪 Target exists now: '${_IS_GLOBAL}'")

register_target(catalyst.core)