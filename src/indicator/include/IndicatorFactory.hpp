#pragma once

#include <map>
#include <memory>
#include "IIndicator.hpp"
#include "LogMacros.h"
#include "BacktestConfig.hpp"
#include "SimpleMovingAverage.hpp"
#include "ExponentialMovingAverage.hpp"

namespace indicator
{
	class IndicatorFactory
	{
	public:
		IndicatorFactory();
		std::unique_ptr<IIndicator> CreateIndicator(const std::string& indicatorName, const config::IndicatorParams& indicatorParams);
	private:
		enum eIndicator
		{
			SMA,
			EMA,
			RSI,
			MACD,
			NA
		};
		std::map<eIndicator, std::string> m_indicatorMap;
		eIndicator GetIndicator(const std::string& indicator);
	};
} // namespace indicator
