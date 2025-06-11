#pragma once

#include <vector>
#include "IIndicator.hpp"
#include "BacktestConfig.hpp"
#include "LogMacros.h"

namespace indicator
{
	class SimpleMovingAverage : public IIndicator
	{
	public:
		SimpleMovingAverage(const config::IndicatorParams&);
		~SimpleMovingAverage() = default;
		const IndicatorValues CalculateMovingAverage(const quote::StockQuote& stockQuote);
	private:
		const IndicatorValues Calculate(const quote::StockQuote& stockQuote) override;
		const config::IndicatorParams& m_params = {};
		uint8_t						   m_period = { 0 };
	};
} // namespace indicator
