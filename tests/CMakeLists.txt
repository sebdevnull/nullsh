# ---- Test target name ----
set(NULLSH_TESTS "${PROJECT_NAME}_tests")

# ---- Fetch GoogleTest ----
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Disable clang-tidy for gtest
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

# ---- Test executable ----
add_executable(${NULLSH_TESTS}
    test_util.cpp
    test_cli.cpp
    test_shell.cpp
    test_parser.cpp
    test_command.cpp
    test_result_capturer.cpp
    test_builtins.cpp
    test_executor.cpp)

set_target_properties(${NULLSH_TESTS} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}-tests
)

target_link_libraries(${NULLSH_TESTS}
    PRIVATE
    ${NULLSH_LIB}
    GTest::gtest_main)

# ---- GoogleTest integration ----
include(GoogleTest)
gtest_discover_tests(${NULLSH_TESTS})
