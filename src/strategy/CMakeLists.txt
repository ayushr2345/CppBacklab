# strategy/CMakeLists.txt

add_library(strategy
    src/SMASupportStrategy.cpp
    src/EMASupportStrategy.cpp
    src/StrategyFactory.cpp)

target_include_directories(strategy
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(strategy
    PRIVATE
        utils
        config
        quote
        indicator
)
