cmake_minimum_required(VERSION 3.27)

project(nullsh
    VERSION 0.1.0
    DESCRIPTION "NullShell: A minimalist Linux shell where everything goes into the void"
    LANGUAGES CXX)

# ---- Project-wide C++ standard ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Targets ----
set(NULLSH_LIB ${PROJECT_NAME}_lib)
set(NULLSH_APP ${PROJECT_NAME})

# Library target
add_library(${NULLSH_LIB} STATIC)
target_sources(${NULLSH_LIB} PRIVATE
    src/shell.cpp
    src/util.cpp
    src/cli.cpp
    src/parser.cpp
    src/command.cpp
    src/result_capturer.cpp
    src/executor.cpp
    src/builtins.cpp
)

# Expose headers and generated files
target_include_directories(${NULLSH_LIB}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/generated
)

# Ensure artifact name is clean (libnullsh.a instead of libnullsh_lib.a)
set_target_properties(${NULLSH_LIB} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    POSITION_INDEPENDENT_CODE ON
)

# Executable target
add_executable(${NULLSH_APP} src/main.cpp)
target_compile_options(${NULLSH_APP} PRIVATE
    -Wall -Wextra -Wpedantic
)
target_link_libraries(${NULLSH_APP} PRIVATE ${NULLSH_LIB})

# ---- Build metadata ----
# Build type (Debug/Release/etc.)
if(CMAKE_BUILD_TYPE)
    set(NULLSH_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
else()
    set(NULLSH_BUILD_TYPE "Unknown")
endif()

# Compiler info
set(NULLSH_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
set(NULLSH_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")

# Try to get current Git commit hash
find_package(Git QUIET)
if(Git_FOUND)
    # Short commit hash
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NULLSH_GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Detect dirty working tree
    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --quiet
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIFF_RESULT
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --cached --quiet
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIFF_CACHED_RESULT
    )

    if(NOT GIT_DIFF_RESULT EQUAL 0 OR NOT GIT_DIFF_CACHED_RESULT EQUAL 0)
        set(NULLSH_GIT_COMMIT "${NULLSH_GIT_COMMIT}-dirty")
    endif()

    # Current branch
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NULLSH_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Closest tag (if any)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NULLSH_GIT_TAG
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(NULLSH_GIT_COMMIT "unknown")
    set(NULLSH_GIT_BRANCH "unknown")
    set(NULLSH_GIT_TAG "unknown")
endif()

# System info
set(NULLSH_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")
set(NULLSH_SYSTEM_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}")

# CMake info
set(NULLSH_CMAKE_VERSION "${CMAKE_VERSION}")

# Build timestamp
string(TIMESTAMP NULLSH_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

# Configure version header
configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/generated/nullsh/version.h
    @ONLY
)

# ---- Tests ----
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ----- Installation -----

# Install executable
install(TARGETS ${NULLSH_APP}
    RUNTIME DESTINATION bin
)
