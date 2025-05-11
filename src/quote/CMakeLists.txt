# quote/CMakeLists.txt

add_library(quote
    src/StockQuote.cpp
    src/QuoteLoader.cpp
)

target_include_directories(quote
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(quote
    PRIVATE
        utils
)
