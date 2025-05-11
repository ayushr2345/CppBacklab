#include "QuoteLoader.hpp"
#include "SimpleMovingAverage.hpp"
#include "SMASupportStrategy.hpp"
#include "LogMacros.h"

int main()
{
	//logger::Logger::GetInstance().SetLogFile("backtester.log");
	//std::string filePath = "D:\\BacktestSuite\\DataDownloader\\data\\DRREDDY\\DRREDDY_NSE_Cash_1d_2000-01-01_to_2025-05-01.csv";
	//std::optional<quote::StockQuote> stockQuote = quote::LoadFromCSV(filePath);

	//if (not stockQuote.has_value())
	//{
	//	LOG_ERROR("main(): Failed to load stock quote from file: {}", filePath);
	//	return 1;
	//}

	//if (not stockQuote.value().IsValid())
	//{
	//	LOG_ERROR("main(): Stock quote is invalid: {}", stockQuote.value().GetTickerName());
	//	return 1;
	//}

	//indicator::SimpleMovingAverage _sma_44(44);

	//indicator::IndicatorValues _sma_44_DrReddy = _sma_44.CalculateMovingAverage(stockQuote.value());

	//if (_sma_44_DrReddy.empty())
	//{
	//	LOG_ERROR("main(): Failed to calculate moving average for stock quote: {}", stockQuote.value().GetTickerName());
	//	return 1;
	//}

	//strategy::SMASupportStrategy _smaSupportStrategy;
	//strategy::SignalArray signals	= _smaSupportStrategy.GenerateSignals(stockQuote.value(), _sma_44_DrReddy);

	//LOG_INFO("main(): Successfully generated signals for stock quote: {}", stockQuote.value().GetTickerName());

	//// Print the signals
	//for (const auto& signal : signals)
	//{
	//	switch (signal)
	//	{
	//	case strategy::eSignal::BUY:
	//		LOG_INFO("Signal: BUY SIGNAL");
	//		break;
	//	case strategy::eSignal::SELL:
	//		LOG_INFO("Signal: SELL SIGNAL");
	//		break;
	//	case strategy::eSignal::HOLD:
	//		LOG_INFO("Signal: HOLD SIGNAL");
	//		break;
	//	case strategy::eSignal::NOT_ENOUGH_DATA:
	//		LOG_INFO("Signal: NOT ENOUGH DATA");
	//		break;
	//	default:
	//		LOG_ERROR("Unknown signal");
	//		break;
	//	}
	//}
	return 0;
}