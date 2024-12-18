
cmake_minimum_required(VERSION 3.10)

#
# De-duplicate libraries on link lines based on linker capabilities.
#
# fixes Xcode complaining:
# ld: warning: ignoring duplicate libraries: 'libcommon.a'
#
cmake_policy(SET CMP0156 NEW)

project(advent2024)


enable_testing()


add_subdirectory(day1)

add_subdirectory(day1part2)

add_subdirectory(day2)

add_subdirectory(day2part2)

add_subdirectory(day3)

add_subdirectory(day3part2)

add_subdirectory(day4)

add_subdirectory(day4part2)

add_subdirectory(day5)

add_subdirectory(day5part2)

add_subdirectory(day6)

add_subdirectory(day6part2)

add_subdirectory(day7)

add_subdirectory(day7part2)

add_subdirectory(day8)

add_subdirectory(day8part2)

add_subdirectory(day12)

add_subdirectory(day12part2)

add_subdirectory(day16)

add_subdirectory(day16part2)

add_subdirectory(day18)

add_subdirectory(day18part2)
















