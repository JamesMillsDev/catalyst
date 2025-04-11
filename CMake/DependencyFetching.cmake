include(FetchContent)

function(fetch_dependencies)
    # Define dependencies
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
    )
    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG glad2
    )
    FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
    )
    FetchContent_Declare(
        reactphysics3d
        GIT_REPOSITORY https://github.com/DanielChappuis/reactphysics3d.git
    )

    # Header-only dependencies
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
    )
    FetchContent_Declare(
        ryml
        GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
    )
    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
    )
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
    )
endfunction()