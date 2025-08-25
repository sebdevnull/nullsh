# ---- Test target name ----
set(NULLSH_TESTS "${PROJECT_NAME}_tests")

# ---- Fetch GoogleTest ----
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---- Test executable ----
add_executable(${NULLSH_TESTS}
    test_util.cpp
    test_cli.cpp
    test_shell.cpp
    test_parser.cpp
    test_command.cpp
    test_result_capturer.cpp)

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
