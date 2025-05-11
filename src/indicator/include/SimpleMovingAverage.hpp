#include <vector>
#include "IIndicator.hpp"
#include "LogMacros.h"

namespace indicator
{
	class SimpleMovingAverage : public IIndicator
	{
	public:
		SimpleMovingAverage(const uint8_t period = 44);
		~SimpleMovingAverage() = default;
		const IndicatorValues CalculateMovingAverage(const quote::StockQuote& stockQuote);
	private:
		const IndicatorValues Calculate(const quote::StockQuote& stockQuote) override;
		const uint8_t	       m_period = { 44 };
	};
} // namespace indicator
