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
		LOG_INFO("SMASupportStrategy::SMASupportStrategy(): Created SMA Support Strategy with strategy config: {} and indicator configs: {}",
				  m_strategyConfig.name, m_indicatorConfigs.size());
	}

	const SignalArray SMASupportStrategy::GenerateSignals(const quote::StockQuote& stockQuote)
	{
		if (not stockQuote.IsValid())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): StockQuote provided is invalid, returning empty signals vector.");
			return SignalArray();
		}

		std::vector<std::unique_ptr<indicator::IIndicator>> indicators;
		for (auto const& indicatorConfig : m_indicatorConfigs)
		{
			std::unique_ptr<indicator::IIndicator> smaIndicator = m_indicatorFactory.CreateIndicator(indicatorConfig.name, indicatorConfig.params);
			if (not smaIndicator)
			{
				LOG_ERROR("SMASupportStrategy::GenerateSignals(): Could not create indicator: ", indicatorConfig.name);
				return SignalArray();
			}
			else
			{
				indicators.push_back(std::move(smaIndicator));
			}
		}

		// As this is SMA support strategy, need only 1 indicator inside the vector of indicators
		if (indicators.empty())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): No indicator geneated from the factory, returning empty signals vector.");
			return SignalArray();
		}
		if (indicators.size() > 1)
		{
			LOG_WARN("SMASupportStrategy::GenerateSignals(): More than one indicator generated from the factory, using only the first one.");
		}

		// Calculate SMA values
		indicator::IndicatorValues sma = indicators.at(0).get()->Calculate(stockQuote);
		if (sma.empty())
		{
			LOG_ERROR("SMASupportStrategy::GenerateSignals(): Could not calculate the SMA values for stock quote: {}",
					   stockQuote.GetTickerName());
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
