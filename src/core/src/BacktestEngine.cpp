#include "BacktestEngine.hpp"
#include "LogMacros.h"
#include "QuoteLoader.hpp"
#include <fstream>
#include <map>

namespace core
{
	BacktestEngine::BacktestEngine()
	{
	}

	BacktestEngine::~BacktestEngine()
	{
	}

	bool BacktestEngine::Run(const config::BacktestConfig& config)
	{
		if (not config.IsValid())
		{
			LOG_ERROR("The backtester config provided to the function is not valid. Please check the configuration");
			return false;
		}

		LOG_INFO("BacktestEngine::Run(): Starting backtest with strategy: {} and {} csv files",
				  config.strategy.name, config.csvFiles.size());

		std::map<std::string, quote::StockQuote> stockQuoteMap;
		for (const auto& csvFile : config.csvFiles)
		{
			LOG_INFO("BacktestEngine::Run(): Loading stock quote from file: {}", csvFile);
			auto stockQuote = quote::LoadFromCSV(csvFile);
			if (not stockQuote.has_value())
			{
				LOG_ERROR("BacktestEngine::Run(): Failed to load stock quote from file: {}", csvFile);
				return false;
			}
			if (not stockQuote.value().IsValid())
			{
				LOG_ERROR("BacktestEngine::Run(): Stock quote is invalid: {}", stockQuote.value().GetTickerName());
				return false;
			}
			LOG_INFO("BacktestEngine::Run(): Successfully loaded stock quote: {}", stockQuote.value().GetTickerName());
			stockQuoteMap.insert({ stockQuote.value().GetTickerName(), stockQuote.value() });
		}

		const auto strategyFromConfig = m_strategyFactory.CreateStrategy(config);
		if (not strategyFromConfig)
		{
			LOG_ERROR("BacktestEngine::Run(): Failed to create strategy from config: {}", config.strategy.name);
			return false;
		}

		LOG_INFO("BacktestEngine::Run(): Successfully loaded {} stock quotes", stockQuoteMap.size());
		for (const auto& [tickerName, stockQuote] : stockQuoteMap)
		{
			LOG_INFO("BacktestEngine::Run(): Running backtest on stock quote: {} with time frame: {} and period: {}",
					  tickerName, TimeFrameEnumToString(stockQuote.GetTimeFrame()), stockQuote.GetPeriodInYrs());
			strategyFromConfig.get()->GenerateSignals(stockQuote);
		}

		
		return true;
	}
} // namespace backtest
