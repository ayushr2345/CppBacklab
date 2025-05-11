# indicator/CMakeLists.txt

add_library(indicator
    src/SimpleMovingAverage.cpp
    src/ExponentialMovingAverage.cpp "src/IndicatorFactory.cpp")

target_include_directories(indicator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(indicator
    PRIVATE
        quote
        utils
        config
)