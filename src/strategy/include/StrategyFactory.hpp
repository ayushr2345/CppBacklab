#pragma once

#include <map>
#include "IStrategy.hpp"
#include "BacktestConfig.hpp"
#include "ConfigParser.hpp"

namespace strategy
{
	class StrategyFactory
	{
	public:
		StrategyFactory();
		std::unique_ptr<IStrategy> CreateStrategy(const config::BacktestConfig& config);
	private:
		enum eStrategy
		{
			SMA_SUPPORT,
			EMA_SUPPORT,
			SMA_CROSSOVER,
			EMA_CROSSOVER,
			EMA_SMA_CROSSOVER,
			NA
		};
		std::map<eStrategy, std::string> m_strategyMap;
		eStrategy GetStrategy(const std::string& strategy);
	};
} // namespace strategy
