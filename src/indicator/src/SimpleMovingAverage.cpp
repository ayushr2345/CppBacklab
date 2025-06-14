#include <iostream>
#include "SimpleMovingAverage.hpp"

namespace indicator
{
    SimpleMovingAverage::SimpleMovingAverage(const config::IndicatorParams& params):
        m_params (params)
    {
		auto periodPtr = m_params.find("period");
		if (periodPtr == m_params.end())
		{
			LOG_ERROR("SimpleMovingAverage::SimpleMovingAverage(): Period not found in params, returning.");
			return;
		}
		m_period = static_cast<uint8_t>(periodPtr->second);
		LOG_INFO("SimpleMovingAverage::SimpleMovingAverage(): Created Simple Moving Average Indicator with period: {}", m_period);
    }

    const IndicatorValues SimpleMovingAverage::Calculate(const quote::StockQuote& stockQuote)
    {
        if (stockQuote.IsValid() == false)
        {
            LOG_ERROR("SimpleMovingAverage::Calculate(): StockQuote provided is invalid, returning.");
            return IndicatorValues();
        }

        IndicatorValues simpleMovingAverageList;
        // Initial values will be 0 for the first period days
        for (uint16_t i = 0; i < m_period - 1; i++)
        {
            simpleMovingAverageList.push_back(0);
        }

        // Using Sliding Window Technique
        // Calculate the moving average for the first period days
        double sum = 0.0;
        const quote::ClosePrices& closePrices = stockQuote.GetClosePrices();

        for (uint16_t i = 0; i < m_period; i++)
        {
            sum += closePrices[i];
        }
        simpleMovingAverageList.push_back(sum / m_period);

        size_t pricesSize = closePrices.size();

        // Calculate the moving average for rest of the days
        for (size_t i = m_period; i < pricesSize; i++)
        {
            sum -= closePrices[i - m_period];
            sum += closePrices[i];
            simpleMovingAverageList.push_back(sum / m_period);
        }
        return simpleMovingAverageList;
    }

    const IndicatorValues SimpleMovingAverage::CalculateMovingAverage(const quote::StockQuote& stockQuote)
    {
        const IndicatorValues& indicatorValues = Calculate(stockQuote);
        if (indicatorValues.empty())
        {
            LOG_ERROR("SimpleMovingAverage::CalculateMovingAverage(): Could not calculate the indicator values");
        }

        return indicatorValues;
    }
} // namespace indicators