#include "IndicatorFactory.hpp"

namespace indicator
{
	IndicatorFactory::IndicatorFactory()
	{
		m_indicatorMap.insert({ SMA, "SMA" });
		m_indicatorMap.insert({ EMA, "EMA" });
		m_indicatorMap.insert({ RSI, "RSI" });
		m_indicatorMap.insert({ MACD, "MACD" });
	}

	IndicatorFactory::eIndicator IndicatorFactory::GetIndicator(const std::string& indicator)
	{
		if (indicator == "SMA") return eIndicator::SMA;
		else if (indicator == "EMA") return eIndicator::EMA;
		else if (indicator == "RSI") return eIndicator::RSI;
		else if (indicator == "MACD") return eIndicator::MACD;
		return eIndicator::NA;
	}

	std::unique_ptr<IIndicator> IndicatorFactory::CreateIndicator(const std::string& indicatorName, const config::IndicatorParams& indicatorParams)
	{
		switch (GetIndicator(indicatorName))
		{
		case SMA:
		{
			LOG_INFO("IndicatorFactory::CreateIndicator(): Created Simple Moving Average Indicator from factory.");
			// TODO: MAKE sure the name matches the one provided in the config file
			// TODO: Make the same changes for EMA also
			return std::make_unique<SimpleMovingAverage>(indicatorParams.find("period"));
		}
		case EMA:
		{
			LOG_INFO("IndicatorFactory::CreateIndicator(): Created Exponential Moving Average Indicator from factory.");
			return std::make_unique<ExponentialMovingAverage>();
		}
		default:
		{
			LOG_ERROR("IndicatorFactory::CreateIndicator(): Indicator does not exit");
			return nullptr;
		}
		}
	}
} // namespace indicator
