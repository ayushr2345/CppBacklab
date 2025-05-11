#pragma once

#include <string>
#include <memory>
#include "BacktestConfig.hpp"
#include "StrategyFactory.hpp"

namespace core
{
    class BacktestEngine
    {
    public:
        BacktestEngine();
        ~BacktestEngine();

        bool Run(const config::BacktestConfig& config);
    private:
        strategy::StrategyFactory m_strategyFactory;
    };
} // namespace core
