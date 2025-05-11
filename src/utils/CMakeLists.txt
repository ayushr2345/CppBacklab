# utils/CMakeLists.txt

add_library(utils
    src/Logger.cpp
)

target_include_directories(utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# External libs
target_link_libraries(utils PUBLIC fmt::fmt)
