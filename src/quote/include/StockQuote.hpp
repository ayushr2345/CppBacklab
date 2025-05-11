#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace quote {
    using Period = uint8_t;
    using Price  = double;
    using Volume = double;

    using OpenPrices  = std::vector<Price>;
    using HighPrices  = std::vector<Price>;
    using LowPrices   = std::vector<Price>;
    using ClosePrices = std::vector<Price>;
    using Volumes     = std::vector<Volume>;
    using Dates       = std::vector<std::string>;

	enum eTimeFrame : uint8_t
    {
        TF_1m,
        TF_5m,
        TF_15m,
        TF_30m,
        TF_1h,
        TF_1d
    };

	inline std::string TimeFrameEnumToString(const eTimeFrame& timeFrame)
	{
		switch (timeFrame)
		{
		case TF_1m:   return "1m";
		case TF_5m:   return "5m";
		case TF_15m:  return "15m";
		case TF_30m:  return "30m";
		case TF_1h:   return "1h";
		case TF_1d:   return "1d";
		default:      return "Unknown Time Frame";
		}
	}

    class StockQuote
    {
    public:
        StockQuote(
            const std::string tickerName,
            const Period timePeriodInYears,
            const eTimeFrame timeFrame);
        ~StockQuote();

        bool ValidateStockQuote() const;
        bool CreateStockQuote(
            const std::string& tickerName,
            const Dates& dates,
            const OpenPrices& openPrices,
            const HighPrices& highPrices,
            const LowPrices& lowPrices,
            const ClosePrices& closePrices,
            const Volumes& volumes);

        bool               IsValid() const;
        void               DisplayQuoteData() const;
        const std::string  GetTickerName() const;
        const ClosePrices& GetClosePrices() const;
        const Period&      GetPeriodInYrs() const;
        const eTimeFrame&  GetTimeFrame() const;
    private:
        bool         m_isValid { false };
        std::string  m_tickerName;
        Dates        m_dates;
        OpenPrices   m_openPrices;
        HighPrices   m_highPrices;
        LowPrices    m_lowPrices;
        ClosePrices  m_closePrices;
        Volumes      m_volumes;
        Period       m_periodInYrs { 0 };
        eTimeFrame   m_timeFrame { eTimeFrame::TF_1d };
    };
} // namespace quote
