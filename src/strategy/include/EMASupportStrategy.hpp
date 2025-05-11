#pragma once

#include "IStrategy.hpp"
#include "BacktestConfig.hpp"

namespace strategy
{
	class EMASupportStrategy : public IStrategy
	{
	public:
		EMASupportStrategy(const config::StrategyParams& strategyParams, const config::IndicatorConfigs& indicatorConfigs);
		~EMASupportStrategy() = default;
		const SignalArray GenerateSignals(
			const quote::StockQuote& stockQuote,
			const indicator::IndicatorValues& sma) override;
	};
} // namespace strategy
