cmake_minimum_required(VERSION 3.16)
project(beatbop C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# safety's

option(BEATBOP_CLANG_TIDY "Enable clang-tidy" ON)
if (BEATBOP_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if (CLANG_TIDY_EXE)
        set(_TIDY_CHECKS
            clang-analyzer-*
            bugprone-*
            performance-*
            portability-*
            readability-*
            -readability-magic-numbers
            -readability-identifier-length
            -readability-braces-around-statements
            -cppcoreguidelines-*
            -hicpp-*
            -modernize-*
        )
        string(REPLACE ";" "," _TIDY_CHECKS_STR "${_TIDY_CHECKS}")

        set(CMAKE_CXX_CLANG_TIDY
            ${CLANG_TIDY_EXE};
            -checks=${_TIDY_CHECKS_STR};
            -header-filter=^${CMAKE_SOURCE_DIR}/
        )
    endif()
endif()


file(GLOB_RECURSE SOURCES
    "${CMAKE_SOUCR_DIR}/dependencies/src/*.cpp"
    "${CMAKE_SOUCR_DIR}/dependencies/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/glad/src/*.c"
)

add_executable(beatbop ${SOURCES})

target_include_directories(beatbop PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/glad/include"
    "${CMAKE_SOUCR_DIR}/dependencies/include"
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)


target_link_libraries(beatbop PRIVATE OpenGL::GL glfw glm::glm)

