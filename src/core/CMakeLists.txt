# core/CMakeLists.txt

add_library(core
    src/BacktestEngine.cpp)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(core
    PRIVATE
        quote
        utils
        config
        strategy
        indicator
)