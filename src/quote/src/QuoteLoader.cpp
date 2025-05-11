#include <filesystem>
#include <iostream>
#include "QuoteLoader.hpp"
#include "LogMacros.h"
#include "StockQuote.hpp"

namespace quote
{
	// Struct to store file info
	struct FileInfo
	{
		std::string tickerName;
		std::string exchange;
		std::string exchangeSegment;
		std::string timeFrame;
		std::string fromDate;
		std::string toDate;

		bool IsValid()
		{
			return !tickerName.empty() && !exchange.empty() && !exchangeSegment.empty() &&
				!timeFrame.empty() && !fromDate.empty() && !toDate.empty();
		}
	};

	static std::optional<FileInfo> ExtractFileInfoFromFileName(const std::string& fileName)
	{
		std::string fileNameWithoutExtension = fileName.substr(0, fileName.find_last_of('.'));

		std::vector<std::string> parts;
		size_t start = 0;
		size_t end;
		while ((end = fileNameWithoutExtension.find('_', start)) != std::string::npos)
		{
			parts.push_back(fileNameWithoutExtension.substr(start, end - start));
			start = end + 1;
		}
		parts.push_back(fileNameWithoutExtension.substr(start));

		if (parts.size() != 7)
		{
			LOG_ERROR("ExtractFileInfoFromFileName(): Invalid file name format: {}", fileName);
			return std::nullopt;
		}

		FileInfo fileInfo;
		fileInfo.tickerName		 = parts[0];
		fileInfo.exchange	     = parts[1];
		fileInfo.exchangeSegment = parts[2];
		fileInfo.timeFrame		 = parts[3];
		fileInfo.fromDate		 = parts[4];
		// parts[5] will be the string "to"
		fileInfo.toDate			 = parts[6];

		if (!fileInfo.IsValid())
		{
			LOG_ERROR("ExtractFileInfoFromFileName(): Invalid file info extracted from file name: {}", fileName);
			return std::nullopt;
		}
		return fileInfo;
	}

	// Extract filename from filepath
	static std::optional<std::string> ExtractCsvName(const std::string& filePath)
	{
		if (filePath.empty())
		{
			LOG_ERROR("ExtractCsvName(): File path is empty.");
			return std::nullopt;
		}

		// Find the last occurrence of '/' or '\\'
		size_t pos = filePath.find_last_of("/\\");
		if (pos == std::string::npos)
		{
			return filePath;
		}

		return filePath.substr(pos + 1);
	}


	// Function to read Date, Open, High, Low, Close, Volume from extracted csv line
	static bool ReadToken(std::stringstream& line, std::string& token)
	{
		if (line.str().empty())
		{
			LOG_ERROR("ReadToken(): Stringstream is empty. Returning from here");
			return false;
		}
		if (!std::getline(line, token, ','))
		{
			if (line.eof())
				LOG_DEBUG("ReadToken(): Reached end of line.");
			else if (line.fail())
				LOG_ERROR("ReadToken(): Token read failed.");
			else if (line.bad())
				LOG_ERROR("ReadToken(): Line stream is in a bad state.");
			else
				LOG_ERROR("ReadToken(): Unknown read error.");
			return false;
		}
		return true;
	}

	// Funtion to read a line from csv file
	static bool ReadLine(std::ifstream& file, std::string& line)
	{
		if (!file.is_open())
		{
			LOG_ERROR("ReadLine(): File is not open.");
			return false;
		}

		if (not std::getline(file, line))
		{
			if (file.eof())
				LOG_DEBUG("ReadLine(): Reached end of file.");
			else if (file.fail())
				LOG_ERROR("ReadLine(): File read failed.");
			else if (file.bad())
				LOG_ERROR("ReadLine(): File is in a bad state.");
			else
				LOG_ERROR("ReadLine(): Unknown read error.");
			return false;
		}

		return true;
	}

	// Function to convert time frame string to enum
	static std::optional<eTimeFrame> TimeFrameEnumToString(const std::string& timeFrameString)
	{
		if (timeFrameString == "1m")
			return eTimeFrame::TF_1m;
		else if (timeFrameString == "5m")
			return eTimeFrame::TF_5m;
		else if (timeFrameString == "15m")
			return eTimeFrame::TF_15m;
		else if (timeFrameString == "30m")
			return eTimeFrame::TF_30m;
		else if (timeFrameString == "1h")
			return eTimeFrame::TF_1h;
		else if (timeFrameString == "1d")
			return eTimeFrame::TF_1d;
		else
			return std::nullopt;
	}

	// Function to calculate time difference in years
	static std::optional<uint8_t> CalculateTimeDifference(const std::string& fromDate, const std::string& toDate)
	{
		if (fromDate.empty())
		{
			LOG_ERROR("CalculateTimeDifference(): From date is empty.");
			return std::nullopt;
		}
		if (toDate.empty())
		{
			LOG_ERROR("CalculateTimeDifference(): To date is empty.");
			return std::nullopt;
		}

		uint8_t fromYear = 0;
		uint8_t toYear = 0;

		try
		{
			fromYear = std::stoi(fromDate.substr(0, 4));
			toYear = std::stoi(toDate.substr(0, 4));
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("CalculateTimeDifference(): Failed to parse date: {}. Error: {}", fromDate, e.what());
			return std::nullopt;
		}
		return toYear - fromYear;
	}

	std::optional<StockQuote> LoadFromCSV(const std::string& filePath)
	{
		if (filePath.empty())
		{
			LOG_ERROR("LoadFromCSV(): File path provided is empty. Returning nullopt.");
			return std::nullopt;
		}
		if (not std::filesystem::exists(filePath))
		{
			LOG_ERROR("LoadFromCSV(): File does not exist: {}. Returning nullopt.", filePath);
			return std::nullopt;
		}

		LOG_INFO("LoadFromCSV(): File: {} exists, opening file.", filePath);
		std::ifstream file(filePath);
		if (not file.is_open())
		{
			LOG_ERROR("LoadFromCSV(): Failed to open file: {}. Returning nullopt.", filePath);
			return std::nullopt;
		}

		LOG_INFO("LoadFromCSV(): File: {} opened successfully.", filePath);

		std::string line;
		uint16_t lineNumber = 0;

		// Skip the header line
		bool resHeader = ReadLine(file, line);
		if (not resHeader)
		{
			LOG_ERROR("LoadFromCSV(): Failed to read header line. Returning nullopt.");
			return std::nullopt;
		}
		lineNumber++;

		quote::Dates       dates;
		quote::OpenPrices  openPrices;
		quote::HighPrices  highPrices;
		quote::LowPrices   lowPrices;
		quote::ClosePrices closePrices;
		quote::Volumes     volumes;

		while (ReadLine(file, line))
		{
			lineNumber++;
			std::stringstream ss(line);
			std::string date, openPrice, highPrice, lowPrice, closePrice, volume;

			bool ok = true;

			if (not ReadToken(ss, date))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'Datetime' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			if (not ReadToken(ss, openPrice))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'Open' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			if (not ReadToken(ss, highPrice))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'High' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			if (not ReadToken(ss, lowPrice))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'Low' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			if (not ReadToken(ss, closePrice))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'Close' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			if (not ReadToken(ss, volume))
			{
				LOG_ERROR("LoadFromCSV(): Line {}: Failed to read 'Volume' field. Returning nullopt", lineNumber);
				return std::nullopt;
			}

			dates.push_back(date);
			openPrices.push_back(openPrice.empty() ? 0.0f : std::stod(openPrice));
			highPrices.push_back(highPrice.empty() ? 0.0f : std::stod(highPrice));
			lowPrices.push_back(lowPrice.empty() ? 0.0f : std::stod(lowPrice));
			closePrices.push_back(closePrice.empty() ? 0.0f : std::stod(closePrice));
			volumes.push_back(volume.empty() ? 0.0f : std::stod(volume));
		}

		if (dates.size() != openPrices.size() ||
			dates.size() != highPrices.size() ||
			dates.size() != lowPrices.size() ||
			dates.size() != closePrices.size() ||
			dates.size() != volumes.size())
		{
			LOG_ERROR("LoadFromCSV(): Line {}: Data size mismatch. Returning nullopt", lineNumber);
			return std::nullopt;
		}

		LOG_INFO("LoadFromCSV(): Successfully read {} lines from file: {}", lineNumber, filePath);
		std::optional<std::string> fileName = ExtractCsvName(filePath);
		if (not fileName.has_value())
		{
			LOG_ERROR("LoadFromCSV(): Failed to extract file name from file path: {}. Returning nullopt", filePath);
			return std::nullopt;
		}

		std::optional<FileInfo> fileInfo = ExtractFileInfoFromFileName(fileName.value());
		if (not fileInfo.has_value())
		{
			LOG_ERROR("LoadFromCSV(): Failed to extract file info from file name: {}. Returning nullopt", fileName.value());
			return std::nullopt;
		}

		std::string tickerName = fileInfo.value().tickerName;
		std::string timeFrame  =  fileInfo.value().timeFrame;
		std::string fromDate   = fileInfo.value().fromDate;
		std::string toDate     = fileInfo.value().toDate;

		std::optional<eTimeFrame> timeFrameEnum = TimeFrameEnumToString(timeFrame);
		if (not timeFrameEnum.has_value())
		{
			LOG_ERROR("LoadFromCSV(): Could not convert time frame string: {} to time frame enum. Returning nullopt",
					  timeFrame);
			return std::nullopt;
		}

		std::optional<uint8_t> timePeriodInYears = CalculateTimeDifference(fromDate, toDate);
		if (not timePeriodInYears.has_value())
		{
			LOG_ERROR("LoadFromCSV(): Failed to calculate time difference between {} and {}. Returning nullopt",
				fromDate, toDate);
			return std::nullopt;
		}

		quote::StockQuote stockQuote(tickerName, timePeriodInYears.value(), timeFrameEnum.value());
		stockQuote.CreateStockQuote(
			tickerName,
			dates,
			openPrices,
			highPrices,
			lowPrices,
			closePrices,
			volumes);

		LOG_INFO("LoadFromCSV(): Successfully created {} stock quote for ticker: {}", stockQuote.IsValid(), tickerName);
		if (not stockQuote.IsValid())
		{
			LOG_ERROR("LoadFromCSV(): Stock quote is not valid. Returning nullopt");
			return std::nullopt;
		}

		return stockQuote;
	}
} // namespace quote
