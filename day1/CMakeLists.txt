
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


add_executable(day1-exe
    main.cpp
)

target_link_libraries(day1-exe
    PRIVATE
        common-lib
)

set_target_properties(day1-exe
    PROPERTIES
        OUTPUT_NAME day1
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
target_compile_options(day1-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_compile_options(day1-exe PRIVATE
    -Wall -Wextra -pedantic -Werror -Wconversion -Wsign-conversion
)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(day1-exe PRIVATE
    #
    # Zc:preprocessor is needed for handling __VA_OPT__(,)
    #
    /Zc:preprocessor /WX /W4
)
endif()

configure_file(example.txt example.txt COPYONLY)
configure_file(input.txt input.txt COPYONLY)
















