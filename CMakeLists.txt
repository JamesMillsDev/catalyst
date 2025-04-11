cmake_minimum_required(VERSION 3.20)
project(Catalyst)

# Store targets in a global property (safe across scopes)
function(register_target target)
    get_property(existing GLOBAL PROPERTY REGISTERED_TARGETS)
    if(NOT existing)
        set(existing "")
    endif()
    list(APPEND existing "${target}")
    set_property(GLOBAL PROPERTY REGISTERED_TARGETS "${existing}")
endfunction()


cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0074 NEW)

include(${CMAKE_SOURCE_DIR}/CMake/DependencyFetching.cmake)
include(${CMAKE_SOURCE_DIR}/CMake/DependencyConfiguration.cmake)

# Enable folders in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Forces a pure standard-compliant build

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

fetch_dependencies()
configure_dependencies()

# Add subdirectories for modules and projects
add_subdirectory(Catalyst/Core)
message(STATUS "📦 Added subdir: Catalyst/Core")

add_subdirectory(Catalyst/Gameplay)
add_subdirectory(Catalyst/Graphics)
add_subdirectory(Catalyst/Input)
add_subdirectory(Catalyst/Maths)
add_subdirectory(Catalyst/Physics)
add_subdirectory(Catalyst/Scripting)

add_subdirectory(Catalyst-Editor)
add_subdirectory(SampleGame)

# Add individual test subdirectories
add_subdirectory(Tests/Maths)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "CatalystEditor")

file(GLOB_RECURSE ALL_ENGINE_HEADERS
    ${CMAKE_SOURCE_DIR}/Catalyst/**/*.h
    ${CMAKE_SOURCE_DIR}/Catalyst-Editor/**/*.h
)

set(GENERATED_COMPONENTS ${CMAKE_CURRENT_BINARY_DIR}/ComponentGen.cpp)

add_custom_command(
    OUTPUT ${GENERATED_COMPONENTS}
    COMMAND python ${CMAKE_SOURCE_DIR}/CMake/gen_component_anchors.py ${GENERATED_COMPONENTS}
    DEPENDS ${ALL_ENGINE_HEADERS}
    COMMENT "Generating ComponentGen.cpp from REGISTER_COMPONENT macros"
)

# Create a custom target that depends on that file
add_custom_target(GenerateComponentRegistry
    DEPENDS ${GENERATED_COMPONENTS}
)

add_library(component_registry STATIC ${GENERATED_COMPONENTS})
target_link_libraries(component_registry
    PRIVATE
        catalyst.core
        catalyst.graphics
        catalyst.maths
        catalyst.input
        catalyst.scripting
        catalyst.physics
        catalyst.gameplay
)

target_link_libraries(CatalystEditor PRIVATE component_registry)

message(STATUS "CMake setup for Catalyst engine completed.")

list(LENGTH ALL_TARGETS TARGET_COUNT)
message(STATUS "🔍 FINAL TARGET COUNT: ${ALL_TARGETS}")
message(STATUS "🌍 CMake root project = ${CMAKE_PROJECT_NAME}")

get_property(ALL_TARGETS GLOBAL PROPERTY TARGETS)

foreach(TGT IN LISTS ALL_TARGETS)
    get_target_property(HAS_RUNTIME ${TGT} MSVC_RUNTIME_LIBRARY)
    if(NOT HAS_RUNTIME)
        set(HAS_RUNTIME "${CMAKE_MSVC_RUNTIME_LIBRARY} (inherited)")
    endif()
    message(STATUS "🔧 Target '${TGT}' → MSVC Runtime: ${HAS_RUNTIME}")
endforeach()

get_property(all_targets GLOBAL PROPERTY REGISTERED_TARGETS)
message(STATUS "🧾 Registered targets:")
foreach(tgt IN LISTS REGISTERED_TARGETS)
    message(STATUS "  → ${tgt}")
endforeach()
