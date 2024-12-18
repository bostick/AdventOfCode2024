
include(FetchContent)

FetchContent_Declare(common
    # GIT_REPOSITORY
    #     https://github.com/bostick/common.git
    # GIT_TAG
    #     v0.1.0
    # GIT_SHALLOW 1
    # GIT_PROGRESS 1
    SOURCE_DIR
        /Users/brenton/development/github/common
        # /media/psf/development/github/common
)
FetchContent_MakeAvailable(common)

FetchContent_Declare(googletest
  GIT_REPOSITORY
    https://github.com/google/googletest.git
  GIT_TAG
    v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)



add_library(day18part2-lib
    day18part2.cpp
)

#
# Setup warnings
#
# https://www.foonathan.net/2018/10/cmake-warnings/
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
target_compile_options(day18part2-lib PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(day18part2-lib PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(day18part2-lib PRIVATE
    #
    # Zc:preprocessor is needed for handling __VA_OPT__(,)
    #
    /Zc:preprocessor /WX /W4
)
endif()

target_link_libraries(day18part2-lib
    PRIVATE
        common-lib
)

set_target_properties(day18part2-lib
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO
)



add_executable(day18part2-exe
    main.cpp
)

target_link_libraries(day18part2-exe
    PRIVATE
        common-lib
        day18part2-lib
)

set_target_properties(day18part2-exe
    PROPERTIES
        OUTPUT_NAME day18part2
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO
)

#
# Setup warnings
#
# https://www.foonathan.net/2018/10/cmake-warnings/
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
target_compile_options(day18part2-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(day18part2-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(day18part2-exe PRIVATE
    #
    # Zc:preprocessor is needed for handling __VA_OPT__(,)
    #
    /Zc:preprocessor /WX /W4
)
endif()



configure_file(example.txt example.txt COPYONLY)
configure_file(input.txt input.txt COPYONLY)


add_executable(day18part2-test-exe
    TestDay18Part2.cpp
)

target_link_libraries(day18part2-test-exe
    PRIVATE
        day18part2-lib
        GTest::gmock_main
        common-lib
)

#
# Setup warnings
#
# https://www.foonathan.net/2018/10/cmake-warnings/
#
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
target_compile_options(day18part2-test-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
target_compile_options(day18part2-test-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(day18part2-test-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(day18part2-test-exe PRIVATE
    #
    # /Zc:preprocessor is needed for handling __VA_OPT__(,)
    #
    /Zc:preprocessor /WX /W4
)
else()
message(FATAL_ERROR "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

set_target_properties(day18part2-test-exe
    PROPERTIES
        OUTPUT_NAME day18part2-test
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO
)


gtest_discover_tests(day18part2-test-exe)













