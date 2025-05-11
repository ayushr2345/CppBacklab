#pragma once

#include <string>
#include <optional>
#include "StockQuote.hpp"

namespace quote
{
    std::optional<StockQuote> LoadFromCSV(const std::string& filePath);
} // namespace quote

