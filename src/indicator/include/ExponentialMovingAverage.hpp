#include <vector>
#include "IIndicator.hpp"
#include "LogMacros.h"
#include "BacktestConfig.hpp"

namespace indicator
{
	class ExponentialMovingAverage : public IIndicator
	{
	public:
		ExponentialMovingAverage(const config::IndicatorParams&);
		~ExponentialMovingAverage() = default;
		const IndicatorValues CalculateMovingAverage(const quote::StockQuote& stockQuote);
	private:
		const IndicatorValues Calculate(const quote::StockQuote& stockQuote) override;
		uint8_t						   m_period = { 0 };
		double						   m_multiplier = { 0 };
		const config::IndicatorParams& m_params = {};
	};
} // namespace indicator
