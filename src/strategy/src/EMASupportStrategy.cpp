#include "EMASupportStrategy.hpp"

namespace strategy
{
	EMASupportStrategy::EMASupportStrategy(const config::StrategyParams& strategyParams, const config::IndicatorConfigs& indicatorConfigs)
	{
	}

	const SignalArray EMASupportStrategy::GenerateSignals(
		const quote::StockQuote& stockQuote,
		const indicator::IndicatorValues& ema)
	{
		if (stockQuote.IsValid())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): StockQuote provided is invalid, returning empty signals vector.");
			return SignalArray();
		}
		if (ema.empty())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): ema vector provided is empty, returning empty signals vector.");
			return SignalArray();
		}

		const quote::ClosePrices& closePrices = stockQuote.GetClosePrices();
		if (closePrices.empty())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): Close prices vector empty from function"
				"quote::StockQuote::GetClosePrices() for quote: {}, returning empty signals vector.",
				stockQuote.GetTickerName());
			return SignalArray();
		}
		if (closePrices.size() != ema.size())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): Close prices size not equal to ema size, returning empty signals vector.");
			return SignalArray();
		}

		SignalArray signalArray(ema.size(), eSignal::HOLD);
		bool openPosition = false;
		for (uint16_t i = 0; i < ema.size(); i++)
		{
			if (ema[i] == 0)
			{
				// Not enough data to calculate moving average, set to not enough data and skip
				signalArray[i] = eSignal::NOT_ENOUGH_DATA;
				continue;
			}

			if (closePrices[i] > ema[i] and not openPosition)
			{
				signalArray[i] = eSignal::BUY;
				openPosition = true;
			}
			else if (closePrices[i] < ema[i] and openPosition)
			{
				signalArray[i] = eSignal::SELL;
				openPosition = false;
			}
		}
		return signalArray;
	}
} // namespace strategy
