#pragma once

#include "StockQuote.hpp"

namespace indicator
{
	using IndicatorValues = std::vector<double>;

	class IIndicator
	{
	public:
		virtual ~IIndicator() = default;
		virtual const IndicatorValues Calculate(const quote::StockQuote& stockQuote) = 0;
	};
} // namespace indicator
