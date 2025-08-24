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
add_library(${NULLSH_LIB})
target_sources(${NULLSH_LIB} PRIVATE
    src/shell.cpp
    src/util.cpp
    src/cli.cpp
    src/parser.cpp
    src/command.cpp
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

# ---- Version header ----
configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
    @ONLY
)

target_link_libraries(${NULLSH_APP} PRIVATE ${NULLSH_LIB})

# ---- Tests ----
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
