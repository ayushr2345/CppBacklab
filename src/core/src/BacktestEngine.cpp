#include "BacktestEngine.hpp"
#include "LogMacros.h"
#include <fstream>

namespace core
{
    BacktestEngine::BacktestEngine()
    {
    }

    BacktestEngine::~BacktestEngine()
    {
    }

    bool BacktestEngine::Run(const config::BacktestConfig& config)
    {
        if (not config.IsValid())
        {
            LOG_ERROR("The backtester config provided to the function is not valid. Please check the configuration");
            return false;
        }

        const auto strategyFromConfig = m_strategyFactory.CreateStrategy(config);
        return true;
    }
} // namespace backtest
