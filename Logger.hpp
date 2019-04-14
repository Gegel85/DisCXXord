//
// Created by Gegel85 on 10/04/2019.
//

#ifndef DISC_ORD_LOGGER_HPP
#define DISC_ORD_LOGGER_HPP


#include <string>
#include <fstream>

namespace DisCXXord {
	class Logger {
	public:
		enum LogLevel {
			DEBUG,
			INFO,
			WARNING,
			ERR,
			CRITICAL,
			NONE,
		};

		enum Color {
			WHITE,
			BLUE,
			YELLOW,
			RED,
			DARK_RED,
		};

		class OpenErrorException : public std::exception {
		private:
			std::string	_msg;
		public:
			explicit OpenErrorException(const std::string &msg) : _msg(msg) {};
			const char *what() const noexcept override { return this->_msg.c_str(); };
		};

		Logger(const std::string &path, LogLevel level);
		void debug(const std::string &msg);
		void info(const std::string &msg);
		void warning(const std::string &msg);
		void error(const std::string &msg);
		void critical(const std::string &msg);
		void setTextColor(Color color);

	private:
		LogLevel	_logLevel;
		std::ofstream	_file;
	};
}


#endif //DISC_ORD_LOGGER_HPP
