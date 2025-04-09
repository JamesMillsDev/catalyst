cmake_minimum_required(VERSION 3.20)

project(MathsTests)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    source/*.cpp
    include/*.h
)

add_executable(MathsTests ${SOURCE_FILES})

target_include_directories(MathsTests
    PUBLIC include
    PRIVATE source
)

target_link_libraries(MathsTests
    PRIVATE
        catalyst.maths
)

set_target_properties(MathsTests PROPERTIES FOLDER "Tests")
