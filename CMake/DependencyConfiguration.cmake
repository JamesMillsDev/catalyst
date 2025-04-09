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

function(configure_glad)
    status_message("=== Configuring: GLAD ===")
        
    # Ensure GLAD is built correctly
    FetchContent_GetProperties(glad)
    if(NOT glad_POPULATED)
        FetchContent_MakeAvailable(glad)

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