function(status_message msg)
    if(NOT WIN32)
        # ANSI escape code for green: \033[1;32m ... \033[0m
        message(STATUS "\033[1;32m${msg}\033[0m")
    else()
        # Fallback for Windows (no ANSI)
        message(STATUS "${msg}")
    endif()
endfunction()

function(add_to_folder TARGET)
    if(TARGET ${TARGET})
        get_target_property(TARGET_TYPE ${TARGET} TYPE)
        if(NOT TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
            set_target_properties(${TARGET} PROPERTIES FOLDER "Dependencies")
        else()
            set_target_properties(${TARGET} PROPERTIES FOLDER "Header-Only")
        endif()
    endif()
endfunction()

function(configure_dependencies)
    configure_assimp()
    configure_pugi()
    configure_glad()
    configure_imgui()

    # List of dependency targets
    set(DEPENDENCY_TARGETS glfw reactphysics3d ryml glm)

    # Loop through the dependency targets and assign them to the "Dependencies" folder
    foreach(TARGET ${DEPENDENCY_TARGETS})
        status_message("=== Configuring: ${TARGET} ===")
        FetchContent_MakeAvailable(${TARGET})
        add_to_folder(${TARGET})
    endforeach()
    
    set(HEADER_DEPENDENCY_TARGETS stb nlohmann_json)

    # Loop through the dependency targets and assign them to the "Dependencies" folder
    foreach(TARGET ${HEADER_DEPENDENCY_TARGETS})
        status_message("=== Configuring: ${TARGET} ===")
        FetchContent_Populate(${TARGET})

        string(CONCAT PATH "${TARGET}_SOURCE_DIR")

        set(PATH "${${PATH}}")

        if(NOT TARGET ${TARGET})
            add_library(${TARGET} INTERFACE)
            target_include_directories(${TARGET} INTERFACE ${PATH})
        endif()

        add_to_folder(${TARGET} "Header-Only")
    endforeach()
endfunction()

function(configure_glad)
    status_message("=== Configuring: GLAD ===")
        
    # Ensure GLAD is built correctly
    FetchContent_GetProperties(glad)
    if(NOT glad_POPULATED)
        FetchContent_Populate(glad)

        # Path to generated GLAD files
        set(GLAD_GENERATED_DIR ${glad_SOURCE_DIR}/generated)

        # Run GLAD's Python build script for OpenGL core 4.6
        execute_process(
            COMMAND python -m glad --api gl:core=4.6 --out-path ${GLAD_GENERATED_DIR}
            WORKING_DIRECTORY ${glad_SOURCE_DIR}
            RESULT_VARIABLE GLAD_RESULT
        )

        if(NOT GLAD_RESULT EQUAL 0)
            message(FATAL_ERROR "GLAD build step failed")
        endif()

        # Collect GLAD source files
        file(GLOB GLAD_SOURCES CONFIGURE_DEPENDS ${GLAD_GENERATED_DIR}/src/*.c)
        file(GLOB GLAD_HEADERS CONFIGURE_DEPENDS ${GLAD_GENERATED_DIR}/include/glad/*.h)

        # Create the GLAD static library
        add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_HEADERS})
        target_include_directories(glad PUBLIC ${GLAD_GENERATED_DIR}/include)
        add_to_folder(glad)
    endif()
endfunction()

function(configure_pugi)
    status_message("=== Configuring: pugixml ===")
        
    FetchContent_Populate(pugixml)

    # Manually create a static library from pugixml source
    add_library(pugixml STATIC
        ${pugixml_SOURCE_DIR}/src/pugixml.cpp
    )

    target_include_directories(pugixml PUBLIC
        ${pugixml_SOURCE_DIR}/src
    )

    add_to_folder(pugixml)
endfunction()

function(configure_imgui)
    status_message("=== Configuring: Dear ImGui ===")
        
    # Fetch ImGui manually (since it has no CMakeLists.txt)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking  # Use the docking branch
        GIT_SHALLOW TRUE
    )
    FetchContent_Populate(imgui) # Clones the repo into imgui_SOURCE_DIR

    set(IMGUI_DIR "${imgui_SOURCE_DIR}")

    # List of all possible backend files (both .cpp and .h)
    set(ALL_BACKENDS 
        "glfw"
        "opengl3"
    )

    # Collect ImGui source files manually
    file(GLOB IMGUI_SOURCES 
        "${IMGUI_DIR}/*.cpp"
        "${IMGUI_DIR}/*.h"
    )

    list(GET ALL_BACKENDS 0 FIRST_BACKEND)

    set(BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${FIRST_BACKEND}.h")
    list(APPEND BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${FIRST_BACKEND}.cpp")
    foreach(backend ${ALL_BACKENDS})
        list(FIND BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${backend}.h" ITEM_INDEX)
        if(ITEM_INDEX EQUAL -1)
            list(APPEND BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${backend}.h")
        endif()
        list(FIND BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${backend}.cpp" ITEM_INDEX)
        if(ITEM_INDEX EQUAL -1)
            list(APPEND BACKENDS "${IMGUI_DIR}/backends/imgui_impl_${backend}.cpp")
        endif()
    endforeach()

    # Create an ImGui static library
    add_library(imgui STATIC ${IMGUI_SOURCES} ${BACKENDS})

    target_link_libraries(imgui PRIVATE glfw)

    # Include directories
    target_include_directories(imgui PUBLIC 
        ${IMGUI_DIR} 
        ${glfw_SOURCE_DIR}/include
    )

    add_to_folder(imgui)
endfunction()

function(configure_assimp)
    status_message("=== Configuring: assimp ===")

    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

    # This is the key flag for /MD
    set(ASSIMP_MSVC_RUNTIME_DYNAMIC ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
    )

    FetchContent_MakeAvailable(assimp)

    # Extra safety (optional): force /MD directly
    foreach(tgt IN LISTS assimp assimp_cmd assimp_view)
        if (TARGET ${tgt})
            set_target_properties(${tgt} PROPERTIES
                MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
            )
            add_to_folder(${tgt})
        endif()
    endforeach()

endfunction()