#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "BacktestConfig.hpp"
#include "LogMacros.h"

namespace config
{
	std::optional<BacktestConfig> LoadConfigFromFile(const std::string& filepath);
} // namespace config
