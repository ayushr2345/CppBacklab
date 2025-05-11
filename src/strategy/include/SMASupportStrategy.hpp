#pragma once

#include "IStrategy.hpp"
#include "IndicatorFactory.hpp"
#include "BacktestConfig.hpp"

namespace strategy
{
	class SMASupportStrategy : public IStrategy
	{
	public:
		SMASupportStrategy(const config::StrategyConfig& strategyConfig, const config::IndicatorConfigs& indicatorConfigs);
		~SMASupportStrategy() = default;
		const SignalArray GenerateSignals(const quote::StockQuote& stockQuote) override;
	private:
		const config::StrategyConfig& m_strategyConfig{};
		const config::IndicatorConfigs& m_indicatorConfigs{};
	};
} // namespace strategy
