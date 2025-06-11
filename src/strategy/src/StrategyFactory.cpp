#include "StrategyFactory.hpp"
#include "SMASupportStrategy.hpp"
#include "EMASupportStrategy.hpp"

namespace strategy
{
	StrategyFactory::StrategyFactory()
	{
		m_strategyMap.insert({ SMA_SUPPORT, "SMA_SUPPORT" });
		m_strategyMap.insert({ EMA_SUPPORT, "EMA_SUPPORT" });
		m_strategyMap.insert({ SMA_CROSSOVER, "SMA_CROSSOVER" });
		m_strategyMap.insert({ EMA_CROSSOVER, "EMA_CROSSOVER" });
		m_strategyMap.insert({ EMA_SMA_CROSSOVER, "SMA_EMA_CROSSOVER" });
	}

	StrategyFactory::eStrategy StrategyFactory::GetStrategy(const std::string& strategy)
	{
		if (strategy == "SMA_SUPPORT") return eStrategy::SMA_SUPPORT;
		else if (strategy == "EMA_SUPPORT") return eStrategy::EMA_SUPPORT;
		else if (strategy == "SMA_CROSSOVER") return eStrategy::SMA_CROSSOVER;
		else if (strategy == "EMA_CROSSOVER") return eStrategy::EMA_CROSSOVER;
		else if (strategy == "EMA_SMA_CROSSOVER") return eStrategy::EMA_SMA_CROSSOVER;
		return eStrategy::NA;
	}

	std::unique_ptr<IStrategy> StrategyFactory::CreateStrategy(const config::BacktestConfig& config)
	{
		if (not config.IsValid())
		{
			LOG_ERROR("StrategyFactory::CreateStrategy(): Backtest config provided to function is invalid.");
			return nullptr;
		}

		std::string strategyName = config.strategy.name;

		switch (GetStrategy(strategyName))
		{
		case SMA_SUPPORT:
		{
			LOG_INFO("StrategyFactory::CreateStrategy(): Creating SMA Support Strategy from factory.");
			return std::make_unique<SMASupportStrategy>(config.strategy, config.indicators);
		}
		case EMA_SUPPORT:
		{
			LOG_INFO("StrategyFactory::CreateStrategy(): Creating EMA Support Strategy from factory.");
			return std::make_unique<EMASupportStrategy>(config.strategy, config.indicators);
		}
		/*case SMA_SUPPORT:
			std::make_unique<SMASupportStrategy>();
		case SMA_SUPPORT:
			std::make_unique<SMASupportStrategy>();
		case SMA_SUPPORT:
			std::make_unique<SMASupportStrategy>();*/
		default:
		{
			LOG_ERROR("StrategyFactory::CreateStrategy(): Strategy does not exit");
			return nullptr;
		}
		}
	}
} // namespace strategy
