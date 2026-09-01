# Catch2 v3, fetched at configure time. Swap GIT_TAG for a newer release as
# desired; the download is cached in the build tree after the first configure.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# One aggregate test binary. Mirror src/flxscalers/<area>/<file>.cpp with a
# tests/cpp/<area>/test_<file>.cpp entry here as the suite grows.
add_executable(flxscalers_cpp_tests
    core/test_matrix.cpp
    scalers/test_min_max_scaler.cpp
)
target_link_libraries(flxscalers_cpp_tests
    PRIVATE
        flxscalers_core
        Catch2::Catch2WithMain
)

# Register each TEST_CASE as an individual CTest entry. Discover in two passes
# so every test name gets an "<area>/" prefix: the core pass takes the
# [matrix]-tagged cases, the scalers pass takes everything else. With
# "cmake.ctest.testSuiteDelimiter": "/" the VS Code Testing view then groups
# them into a tree instead of one flat list. A new scaler's tests need no
# change here as long as they are not tagged [matrix].
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(flxscalers_cpp_tests TEST_PREFIX "core/"    TEST_SPEC "[matrix]")
catch_discover_tests(flxscalers_cpp_tests TEST_PREFIX "scalers/" TEST_SPEC "~[matrix]")
