#pragma once

#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include "LogMacros.h"

namespace config
{
	using StrategyParams  = std::map<std::string, double>;
	using IndicatorParams = std::map<std::string, double>;

	struct IndicatorConfig
	{
		std::string name;
		IndicatorParams params;
	};

	struct StrategyConfig
	{
		std::string name;
		StrategyParams params;
	};

	using IndicatorConfigs = std::vector<IndicatorConfig>;
	struct BacktestConfig
	{
		StrategyConfig strategy;
		IndicatorConfigs indicators;
		std::vector<std::string> csvFiles;

		bool IsValid() const
		{
			// Checking strategy
			if (strategy.name.empty() or strategy.params.empty())
			{
				LOG_ERROR("BacktestConfig::IsValid(): Strategy name is empty or params are empty. Ensure both are given.");
				return false;
			}

			// Checking indicators
			if (indicators.empty())
			{
				LOG_WARN("BacktestConfig::IsValid(): No Indicators are provided. What kind of strategy are you running?");
				return false;
			}

			for (const auto& indicator : indicators)
			{
				if (indicator.name.empty() or indicator.params.empty())
				{
					LOG_ERROR("BacktestConfig::IsValid(): Indicator name is empty or params are empty. Ensure both are given.");
					return false;
				}
			}

			// Checking csvFiles
			if (csvFiles.empty())
			{
				LOG_ERROR("BacktestConfig::IsValid(): No CSV files are given, no data to run backtest on.");
				return false;
			}

			for (const auto& csvFile : csvFiles)
			{
				if (csvFile.empty())
				{
					LOG_ERROR("BacktestConfig::IsValid(): CSV File name cannot be empty. Please check again.");
					return false;
				}
				if (not std::filesystem::exists(csvFile))
				{
					LOG_ERROR("LoadFromCSV(): File does not exist: {}. Returning nullopt.", csvFile);
					return false;
				}
			}
			return true;
		}
	};
} // namesace config
