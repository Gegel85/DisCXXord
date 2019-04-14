//
// Created by Gegel85 on 10/04/2019.
//

#include <string>
#include <iostream>
#include "Logger.hpp"
#include "platform.hpp"

#ifdef _WINDOWS_DISCXXORD
#include <windows.h>
#endif

namespace DisCXXord {

	Logger::Logger(const std::string &path, Logger::LogLevel level) :
		_logLevel(level)
	{
		this->_file.open(path);
		if (this->_file.fail())
			throw OpenErrorException(strerror(errno));
	}

	#ifdef _WINDOWS_DISCXXORD

	void Logger::setTextColor(Logger::Color color)
	{
		HANDLE	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO	lpConsoleScreenBufferInfo;
		WORD	col;

		GetConsoleScreenBufferInfo(hConsole, &lpConsoleScreenBufferInfo);
		col = lpConsoleScreenBufferInfo.wAttributes & 0xF0;
		switch (color) {
		case WHITE:
			SetConsoleTextAttribute(hConsole, col | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case BLUE:
			SetConsoleTextAttribute(hConsole, col | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case RED:
			SetConsoleTextAttribute(hConsole, col | FOREGROUND_RED);
			break;
		case DARK_RED:
			SetConsoleTextAttribute(hConsole, col | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case YELLOW:
			SetConsoleTextAttribute(hConsole, col | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		}
	}

	#else

	void Logger::setTextColor(Logger::Color color)
	{
		switch (color) {
		case WHITE:
			std::cout << "\033[0m";
			break;
		case BLUE:
			std::cout << "\033[96m";
			break;
		case RED:
			std::cout << "\033[91m";
			break;
		case DARK_RED:
			std::cout << "\033[31m";
			break;
		case YELLOW:
			std::cout << "\033[93m";
			break;
		}
	}

	#endif

	void Logger::debug(const std::string &msg)
	{
		if (this->_logLevel <= DEBUG) {
			this->setTextColor(WHITE);
			std::cout << "[DEBUG]: " << msg << std::endl;
			this->_file << "[DEBUG]: " << msg << std::endl;
		}
	}

	void Logger::info(const std::string &msg)
	{
		if (this->_logLevel <= INFO) {
			this->setTextColor(BLUE);
			std::cout << "[INFO]";
			this->setTextColor(WHITE);
			std::cout << ": " << msg << std::endl;
			this->_file << "[INFO]: " << msg << std::endl;
		}
	}

	void Logger::warning(const std::string &msg)
	{
		if (this->_logLevel <= WARNING) {
			this->setTextColor(YELLOW);
			std::cout << "[WARNING]";
			this->setTextColor(WHITE);
			std::cout << ": " << msg << std::endl;
			this->_file << "[WARNING]: " << msg << std::endl;
		}
	}

	void Logger::error(const std::string &msg)
	{
		if (this->_logLevel <= ERR) {
			this->setTextColor(DARK_RED);
			std::cout << "[ERROR]";
			this->setTextColor(WHITE);
			std::cout << ": " << msg << std::endl;
			this->_file << "[ERROR]: " << msg << std::endl;
		}
	}

	void Logger::critical(const std::string &msg)
	{
		if (this->_logLevel <= CRITICAL) {
			this->setTextColor(RED);
			std::cout << "[FATAL]";
			this->setTextColor(WHITE);
			std::cout << ": " << msg << std::endl;
			this->_file << "[FATAL]: " << msg << std::endl;
		}
	}
}