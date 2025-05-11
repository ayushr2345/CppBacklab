#include "SMASupportStrategy.hpp"

namespace strategy
{
	SMASupportStrategy::SMASupportStrategy(
		const config::StrategyConfig& strategyConfig,
		const config::IndicatorConfigs& indicatorConfigs
	) :
		m_strategyConfig   (strategyConfig),
		m_indicatorConfigs (indicatorConfigs)
	{
	}

	const SignalArray SMASupportStrategy::GenerateSignals(const quote::StockQuote& stockQuote)
	{
		if (not stockQuote.IsValid())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): StockQuote provided is invalid, returning empty signals vector.");
			return SignalArray();
		}

		indicator::IndicatorFactory indicatorFactory;
		for (auto const& indicatorConfig : m_indicatorConfigs)
		{
			std::unique_ptr<indicator::IIndicator> smaIndicator = indicatorFactory.CreateIndicator(indicatorConfig.name, indicatorConfig.params);
			if (smaIndicator)
			{

			}
		}

		if (sma.empty())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): SMA vector provided is empty, returning empty signals vector.");
			return SignalArray();
		}

		const quote::ClosePrices& closePrices = stockQuote.GetClosePrices();
		if (closePrices.empty())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): Close prices vector empty from function"
					  "quote::StockQuote::GetClosePrices() for quote: {}, returning empty signals vector.",
					   stockQuote.GetTickerName());
			return SignalArray();
		}
		if (closePrices.size() != sma.size())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): Close prices size not equal to sma size, returning empty signals vector.");
			return SignalArray();
		}

		SignalArray signalArray(sma.size(), eSignal::HOLD);
		bool openPosition = false;
		for (uint16_t i = 0; i < sma.size(); i++)
		{
			if (sma[i] == 0)
			{
				// Not enough data to calculate moving average, set to not enough data and skip
				signalArray[i] = eSignal::NOT_ENOUGH_DATA;
				continue;
			}

			if (closePrices[i] > sma[i] and not openPosition)
			{
				signalArray[i] = eSignal::BUY;
				openPosition = true;
			}
			else if (closePrices[i] < sma[i] and openPosition)
			{
				signalArray[i] = eSignal::SELL;
				openPosition = false;
			}
		}
		return signalArray;
	}
} // namespace strategy
