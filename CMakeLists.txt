cmake_minimum_required(VERSION 3.10)
project(FrameEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0072 NEW)

# GLFW
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
include_directories(external/glad/include)

# OpenGL
find_package(OpenGL REQUIRED)

# fmt through FetchContent if missing
find_package(fmt QUIET)
if(NOT fmt_FOUND)
    message(STATUS "fmt not found, fetching...")
    include(FetchContent)
    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(fmt)
endif()

# Include
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES src/*.cpp)

# Executable
add_executable(frame_engine ${SOURCES})
target_link_libraries(frame_engine OpenGL::GL glfw glad fmt::fmt)

# Testing
enable_testing()
find_package(Catch2 REQUIRED)

add_executable(test_vector3 tests/unit/test_vector3.cpp)
target_link_libraries(test_vector3 PRIVATE Catch2::Catch2WithMain)

add_executable(test_ecs tests/unit/test_ecs.cpp)
target_link_libraries(test_ecs PRIVATE Catch2::Catch2WithMain)

add_test(NAME Vector3Tests COMMAND test_vector3)
add_test(NAME ECSTests COMMAND test_ecs)

# Doxygen Documentation
find_package(Doxygen REQUIRED)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )
endif()
