#pragma once

#include "IStrategy.hpp"
#include "IndicatorFactory.hpp"
#include "BacktestConfig.hpp"

namespace strategy
{
	class EMASupportStrategy : public IStrategy
	{
	public:
		EMASupportStrategy(const config::StrategyConfig& strategyConfig, const config::IndicatorConfigs& indicatorConfigs);
		~EMASupportStrategy() = default;
		const SignalArray GenerateSignals(const quote::StockQuote& stockQuote) override;
	private:
		const config::StrategyConfig&   m_strategyConfig{};
		const config::IndicatorConfigs& m_indicatorConfigs{};
		indicator::IndicatorFactory     m_indicatorFactory{};
	};
} // namespace strategy
