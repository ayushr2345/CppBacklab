#include <iostream>
#include "ExponentialMovingAverage.hpp"

namespace indicator
{
    ExponentialMovingAverage::ExponentialMovingAverage(const uint8_t period) :
        m_period(period)
    {
		m_multiplier = 2.0 / (m_period + 1.0);
		LOG_INFO("ExponentialMovingAverage::ExponentialMovingAverage(): period: {}, multiplier: {}", m_period, m_multiplier);
    }

    const IndicatorValues ExponentialMovingAverage::Calculate(const quote::StockQuote& stockQuote)
    {
        if (stockQuote.IsValid() == false)
        {
            LOG_ERROR("ExponentialMovingAverage::Calculate(): StockQuote provided is invalid, returning.");
            return IndicatorValues();
        }

        IndicatorValues exponentialMovingAverageList;
        // Initial values will be 0 for the first period days
        for (uint16_t i = 0; i < m_period - 1; i++)
        {
            exponentialMovingAverageList.push_back(0);
        }

        // Calculate the simple moving average for the first period
        double sum = 0.0;
        const quote::ClosePrices& closePrices = stockQuote.GetClosePrices();
        uint16_t i = 0;
        for (i = 0; i < m_period; i++)
        {
            sum += closePrices[i];
        }
        exponentialMovingAverageList.push_back((sum + closePrices[i]) / m_period);

        // Calculate the exponential moving average for the rest of the days
        for (size_t i = m_period; i < closePrices.size(); i++)
        {
            double ema = (closePrices[i] * m_multiplier) + (exponentialMovingAverageList.back() * (1 - m_multiplier));
            exponentialMovingAverageList.push_back(ema);
        }
        return exponentialMovingAverageList;
    }

    const IndicatorValues ExponentialMovingAverage::CalculateMovingAverage(const quote::StockQuote& stockQuote)
    {
        const IndicatorValues& indicatorValues = Calculate(stockQuote);
        if (indicatorValues.empty())
        {
            LOG_ERROR("ExponentialMovingAverage::CalculateMovingAverage(): Could not calculate the indicator values");
        }

        return indicatorValues;
    }
} // namespace indicators