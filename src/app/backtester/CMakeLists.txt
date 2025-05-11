# app/backtester/CMakeLists.txt

add_executable(backtester
    main.cpp
)

target_include_directories(backtester PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(backtester
    PRIVATE
        quote
        utils
        config
        indicator
        strategy
        core
)