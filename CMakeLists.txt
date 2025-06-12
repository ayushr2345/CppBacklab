cmake_minimum_required(VERSION 3.31)
project(CppBacklab)
set(CMAKE_CXX_STANDARD 20)

# External dependencies
add_subdirectory(external/fmt)
add_subdirectory(external/json)

# Internal modules
add_subdirectory(src/utils)
add_subdirectory(src/config)
add_subdirectory(src/quote)
add_subdirectory(src/indicator)
add_subdirectory(src/strategy)
add_subdirectory(src/core)
add_subdirectory(src/app)
