#include <vector>
#include "IIndicator.hpp"
#include "LogMacros.h"

namespace indicator
{
	class ExponentialMovingAverage : public IIndicator
	{
	public:
		ExponentialMovingAverage(const uint8_t period = 20);
		~ExponentialMovingAverage() = default;
		const IndicatorValues CalculateMovingAverage(const quote::StockQuote& stockQuote);
	private:
		const IndicatorValues Calculate(const quote::StockQuote& stockQuote) override;
		const uint8_t	       m_period = { 20 };
		double			       m_multiplier = { 0 };
	};
} // namespace indicator
