#include <fstream>
#include "ConfigParser.hpp"

namespace config
{
    static std::optional<BacktestConfig> ParseBacktestConfig(const nlohmann::json& config)
    {
        BacktestConfig cfg;
        cfg.strategy.name = config["strategy"];
        for (auto& [k, v] : config["strategy_params"].items())
        {
            cfg.strategy.params[k] = v;
        }

        for (const auto& ind : config["indicators"])
        {
            IndicatorConfig ic;
            ic.name = ind["name"];
            for (auto& [k, v] : ind["params"].items())
            {
                ic.params[k] = v;
            }
            cfg.indicators.push_back(ic);
        }

        for (const auto& file : config["csv_files"])
        {
            cfg.csvFiles.push_back(file);
        }

        if (not cfg.IsValid())
        {
            LOG_ERROR("ParseBacktestConfig(): Could not parse config file");
            return std::nullopt;
        }
        return cfg;
    }

    std::optional<BacktestConfig> LoadConfigFromFile(const std::string& filePath)
    {
        if (filePath.empty())
        {
            LOG_ERROR("LoadConfigFromFile(): File path provided is empty. Returning nullopt.");
            return std::nullopt;
        }
        if (not std::filesystem::exists(filePath))
        {
            LOG_ERROR("LoadConfigFromFile(): File does not exist: {}. Returning nullopt.", filePath);
            return std::nullopt;
        }

        LOG_INFO("LoadConfigFromFile(): File: {} exists, opening file.", filePath);

        std::ifstream file(filePath);
        if (not file.is_open())
        {
            LOG_ERROR("LoadConfigFromFile(): Failed to open file: {}. Returning nullopt.", filePath);
            return std::nullopt;
        }

        LOG_INFO("LoadConfigFromFile(): File: {} opened successfully.", filePath);
        nlohmann::json config;
        file >> config;
        std::optional<BacktestConfig> cfg = ParseBacktestConfig(config);
        if (not cfg.has_value())
        {
            LOG_ERROR("LoadConfigFromFile(): Not able to parse: {} json file to config structure", filePath);
            return std::nullopt;
        }
        return cfg;
    }
} // namespace config
