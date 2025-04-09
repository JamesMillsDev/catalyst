cmake_minimum_required(VERSION 3.20)
project(Catalyst)

include(${CMAKE_SOURCE_DIR}/CMake/DependencyFetching.cmake)
include(${CMAKE_SOURCE_DIR}/CMake/DependencyConfiguration.cmake)

# Enable folders in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Forces a pure standard-compliant build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD") 

fetch_dependencies()

# List of dependency targets
set(DEPENDENCY_TARGETS assimp glfw reactphysics3d ryml glm stb nlohmann_json)

# Loop through the dependency targets and assign them to the "Dependencies" folder
foreach(TARGET ${DEPENDENCY_TARGETS})
    status_message("=== Configuring: ${TARGET} ===")
    FetchContent_MakeAvailable(${TARGET})
    add_to_folder(${TARGET})
endforeach()

configure_pugi()
configure_glad()
configure_imgui()

# Add subdirectories for modules and projects
add_subdirectory(Catalyst/Core)
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

message(STATUS "CMake setup for Catalyst engine completed.")
