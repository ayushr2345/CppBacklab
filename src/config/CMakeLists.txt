# config/CMakeLists.txt

add_library(config
    src/ConfigParser.cpp
)

target_include_directories(config
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(config
    PRIVATE
        utils
)

# External libs
target_link_libraries(config PUBLIC nlohmann_json::nlohmann_json)
