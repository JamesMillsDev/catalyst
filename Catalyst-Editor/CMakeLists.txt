cmake_minimum_required(VERSION 3.20)

project(CatalystEditor)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    source/*.c
    include/*.h
)

add_executable(CatalystEditor ${SOURCE_FILES})

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

target_include_directories(CatalystEditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Core/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Gameplay/include
    ${CMAKE_SOURCE_DIR}/Catalyst/graphics/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Input/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Maths/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Physics/include
    ${CMAKE_SOURCE_DIR}/Catalyst/Scripting/include
    ${glad_SOURCE_DIR}/generated
    ${assimp_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${stb_SOURCE_DIR}
)

target_link_libraries(CatalystEditor
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
        assimp
        pugixml
        stb
)

target_compile_definitions(CatalystEditor PRIVATE IS_EDITOR)

set_target_properties(CatalystEditor PROPERTIES
    FOLDER "Applications"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET CatalystEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:assimp>"              # Full path to the assimp DLL
        "$<TARGET_FILE_DIR:CatalystEditor>"  # Output bin directory
)

add_custom_command(TARGET CatalystEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:glfw>"              # Full path to the glfw DLL
        "$<TARGET_FILE_DIR:CatalystEditor>"  # Output bin directory
)

# Define paths
set(INPUT_XML "${CMAKE_CURRENT_SOURCE_DIR}/config/editor.xml")  # Change to your file
set(OUTPUT_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/EditorConfig.h")  # Store in include folder
set(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}/CMake/embed_file.py")

add_custom_target(EmbedEditorConfig
    COMMAND ${Python_EXECUTABLE} ${PYTHON_SCRIPT} ${INPUT_XML} ${OUTPUT_HEADER}
    COMMENT "Embedding file: ${INPUT_XML} -> ${OUTPUT_HEADER}"
    VERBATIM
)

add_dependencies(CatalystEditor EmbedEditorConfig)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(CatalystEditor PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Create a script to run the executable from the correct directory
    file(WRITE "${CMAKE_BINARY_DIR}/run_catalyst_editor.sh"
        "#!/bin/bash\n"
        "cd \"${CMAKE_SOURCE_DIR}\" && \"${CMAKE_BINARY_DIR}/Catalyst-Editor\"\n"
    )
    execute_process(COMMAND chmod +x "${CMAKE_BINARY_DIR}/run_catalyst_editor.sh")
endif()