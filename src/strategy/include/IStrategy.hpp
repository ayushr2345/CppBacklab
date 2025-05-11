#pragma once

#include <vector>
#include "StockQuote.hpp"
#include "IIndicator.hpp"
#include "LogMacros.h"

namespace strategy
{
	enum class eSignal
	{
		BUY,
		SELL,
		HOLD,
		NOT_ENOUGH_DATA
	};

	using SignalArray = std::vector<eSignal>;
	class IStrategy
	{
	public:
		virtual ~IStrategy() = default;
		virtual const SignalArray GenerateSignals(
			const quote::StockQuote& stockQuote,
			const indicator::IndicatorValues& indicatorValues) = 0;
	};
} // namespace strategy
