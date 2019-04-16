/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Exception.hpp
*/

#ifndef DISCXXORD_EXCEPTION_HPP
#define DISCXXORD_EXCEPTION_HPP


#include <string>

namespace DisCXXord
{
	class Exception : public std::exception {
	private:
		std::string	_msg;
	public:
		explicit Exception(const std::string &msg) : _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class TimeoutException : public Exception {
	public:
		explicit TimeoutException(const std::string &msg) : Exception(msg) {};
	};

	class DisconnectedException : public Exception {
	public:
		explicit DisconnectedException(const std::string &msg) : Exception(msg) {};
	};

	class InvalidHandshakeException : public Exception {
	public:
		explicit InvalidHandshakeException(const std::string &msg) : Exception(msg) {};
	};

	class InvalidPongException : public Exception {
	public:
		explicit InvalidPongException(const std::string &msg) : Exception(msg) {};
	};

	class ConnectionTerminatedException : public Exception {
	private:
		int _code;

	public:
		explicit ConnectionTerminatedException(const std::string &msg, int code) : Exception(msg), _code(code) {};
		int getCode() { return this->_code; };
	};

	class SocketCreationErrorException : public Exception {
	public:
		explicit SocketCreationErrorException(const std::string &msg) : Exception(msg) {};
	};

	class HostNotFoundException : public Exception {
	public:
		explicit HostNotFoundException(const std::string &msg) : Exception(msg) {};
	};

	class ConnectException : public Exception {
	public:
		explicit ConnectException(const std::string &msg) : Exception(msg) {};
	};

	class NotConnectedException : public Exception {
	public:
		explicit NotConnectedException(const std::string &msg) : Exception(msg) {};
	};

	class AlreadyOpenedException : public Exception {
	public:
		explicit AlreadyOpenedException(const std::string &msg) : Exception(msg) {};
	};

	class EOFException : public Exception {
	public:
		explicit EOFException(const std::string &msg) : Exception(msg) {};
	};

	class RoleNotFoundException : public Exception {
	public:
		explicit RoleNotFoundException(const std::string &msg) : Exception(msg) {};
	};

	class MemberNotFoundException : public Exception {
	public:
		explicit MemberNotFoundException(const std::string &msg) : Exception(msg) {};
	};

	class ChannelNotFoundException : public Exception {
	public:
		explicit ChannelNotFoundException(const std::string &msg) : Exception(msg) {};
	};

	class UserNotFoundException : public Exception {
	public:
		explicit UserNotFoundException(const std::string &msg) : Exception(msg) {};
	};
}


#endif //DISCXXORD_EXCEPTION_HPP
