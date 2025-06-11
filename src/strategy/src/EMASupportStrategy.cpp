#include "EMASupportStrategy.hpp"

namespace strategy
{
	EMASupportStrategy::EMASupportStrategy(
		const config::StrategyConfig& strategyConfig,
		const config::IndicatorConfigs& indicatorConfigs
	) :
		m_strategyConfig(strategyConfig),
		m_indicatorConfigs(indicatorConfigs)
	{
		LOG_INFO("EMASupportStrategy::EMASupportStrategy(): Created EMA Support Strategy with strategy config: {} and indicator configs: {}",
			m_strategyConfig.name, m_indicatorConfigs.size());
	}

	const SignalArray EMASupportStrategy::GenerateSignals(const quote::StockQuote& stockQuote)
	{
		if (not stockQuote.IsValid())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): StockQuote provided is invalid, returning empty signals vector.");
			return SignalArray();
		}

		std::vector<std::unique_ptr<indicator::IIndicator>> indicators;
		for (auto const& indicatorConfig : m_indicatorConfigs)
		{
			std::unique_ptr<indicator::IIndicator> emaIndicator = m_indicatorFactory.CreateIndicator(indicatorConfig.name, indicatorConfig.params);
			if (not emaIndicator)
			{
				LOG_ERROR("EMASupportStrategy::GenerateSignals(): Could not create indicator: ", indicatorConfig.name);
				return SignalArray();
			}
			else
			{
				indicators.push_back(std::move(emaIndicator));
			}
		}

		// As this is EMA support strategy, need only 1 indicator inside the vector of indicators
		if (indicators.empty())
		{
			LOG_ERROR("EMASupportStrategy::GenerateSignals(): No indicator geneated from the factory, returning empty signals vector.");
			return SignalArray();
		}
		if (indicators.size() > 1)
		{
			LOG_WARN("EMASupportStrategy::GenerateSignals(): More than one indicator generated from the factory, using only the first one.");
		}

		// Calculate EMA values
		indicator::IndicatorValues ema = indicators.at(0).get()->Calculate(stockQuote);

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
