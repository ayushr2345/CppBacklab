#include "StockQuote.hpp"
#include "LogMacros.h"

namespace quote
{
    StockQuote::StockQuote(
        const std::string tickerName,
        const Period timePeriodInYears,
        const eTimeFrame timeFrame
    ) :
        m_isValid     (false),
        m_tickerName  (tickerName),
        m_dates       (),
        m_closePrices (),
        m_openPrices  (),
        m_lowPrices   (),
        m_highPrices  (),
        m_volumes     (),
        m_periodInYrs (timePeriodInYears),
        m_timeFrame   (timeFrame)
    {
    }

    StockQuote::~StockQuote()
    {
    }

    bool StockQuote::IsValid() const
    {
        return m_isValid;
    }

    const std::string StockQuote::GetTickerName() const
    {
        if (m_isValid)
        {
			return m_tickerName;
        }
		LOG_ERROR("StockQuote::GetTickerName(): StockQuote is not valid, returning empty string");
        return std::string();
    }

    bool StockQuote::CreateStockQuote(
        const std::string& tickerName,
        const Dates& dates,
        const OpenPrices& openPrices,
        const HighPrices& highPrices,
        const LowPrices& lowPrices,
        const ClosePrices& closePrices,
        const Volumes& volumes)
    {
        this->m_tickerName  = tickerName;
        this->m_dates       = dates;
        this->m_openPrices  = openPrices;
        this->m_highPrices  = highPrices;
        this->m_lowPrices   = lowPrices;
        this->m_closePrices = closePrices;
        this->m_volumes     = volumes;
		m_isValid = ValidateStockQuote();
        return m_isValid;
    }

    bool StockQuote::ValidateStockQuote() const
    {
        if (m_tickerName.empty())
        {
            return false;
        }
		if (m_dates.empty() or m_openPrices.empty() or m_highPrices.empty() or
			m_lowPrices.empty() or m_closePrices.empty() or m_volumes.empty())
		{
			return false;
		}
		if (m_dates.size() != m_openPrices.size() or
			m_dates.size() != m_highPrices.size() or
			m_dates.size() != m_lowPrices.size() or
			m_dates.size() != m_closePrices.size() or
			m_dates.size() != m_volumes.size())
		{
			return false;
		}
		return true;
    }

    void StockQuote::DisplayQuoteData() const
    {
        LOG_INFO("Ticker Name: {}, Time Frame: {},  Time Period: {}",
                 m_tickerName,
                 TimeFrameEnumToString(m_timeFrame),
                 m_periodInYrs);

        LOG_DEBUG("Start Date: {}, End Date: {}",
                  m_dates.front(),
                  m_dates.back());
    }

    const ClosePrices& StockQuote::GetClosePrices() const
    {
        return m_closePrices;
    }

	const Period& StockQuote::GetPeriodInYrs() const
	{
		return m_periodInYrs;
	}

	const eTimeFrame& StockQuote::GetTimeFrame() const
	{
		return m_timeFrame;
	}
} // namespace quote
