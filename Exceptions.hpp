#ifndef DISCXXORD_EXCEPTION_HPP
#define DISCXXORD_EXCEPTION_HPP


#include <string>

namespace DisCXXord
{
	class BaseException : public std::exception {
	private:
		std::string	_msg;
	public:
		explicit BaseException(const std::string &msg) : _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class TimeoutException : public BaseException {
	public:
		explicit TimeoutException(const std::string &msg) : BaseException(msg) {};
	};

	class DisconnectedException : public BaseException {
	public:
		explicit DisconnectedException(const std::string &msg) : BaseException(msg) {};
	};

	class InvalidHandshakeException : public BaseException {
	public:
		explicit InvalidHandshakeException(const std::string &msg) : BaseException(msg) {};
	};

	class InvalidPongException : public BaseException {
	public:
		explicit InvalidPongException(const std::string &msg) : BaseException(msg) {};
	};

	class ConnectionTerminatedException : public BaseException {
	private:
		int _code;

	public:
		explicit ConnectionTerminatedException(const std::string &msg, int code) : BaseException(msg), _code(code) {};
		int getCode() { return this->_code; };
	};

	class SocketCreationErrorException : public BaseException {
	public:
		explicit SocketCreationErrorException(const std::string &msg) : BaseException(msg) {};
	};

	class HostNotFoundException : public BaseException {
	public:
		explicit HostNotFoundException(const std::string &msg) : BaseException(msg) {};
	};

	class ConnectException : public BaseException {
	public:
		explicit ConnectException(const std::string &msg) : BaseException(msg) {};
	};

	class NotConnectedException : public BaseException {
	public:
		explicit NotConnectedException(const std::string &msg) : BaseException(msg) {};
	};

	class AlreadyOpenedException : public BaseException {
	public:
		explicit AlreadyOpenedException(const std::string &msg) : BaseException(msg) {};
	};

	class EOFException : public BaseException {
	public:
		explicit EOFException(const std::string &msg) : BaseException(msg) {};
	};

	class NotFoundException : public BaseException {
	public:
		explicit NotFoundException(const std::string &msg) : BaseException(msg) {};
	};

	class RoleNotFoundException : public NotFoundException {
	public:
		explicit RoleNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};

	class MemberNotFoundException : public NotFoundException {
	public:
		explicit MemberNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};

	class ChannelNotFoundException : public NotFoundException {
	public:
		explicit ChannelNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};

	class UserNotFoundException : public NotFoundException {
	public:
		explicit UserNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};

	class GuildNotFoundException : public NotFoundException {
	public:
		explicit GuildNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};;

	class MessageNotFoundException : public NotFoundException {
	public:
		explicit MessageNotFoundException(const std::string &msg) : NotFoundException(msg) {};
	};

	class InvalidURLException : public BaseException {
	public:
		explicit InvalidURLException(const std::string &msg) : BaseException(msg) {};
	};

	class InvalidChannelException : public BaseException {
	public:
		explicit InvalidChannelException(const std::string &msg) : BaseException(msg) {};
	};

	class NotImplementedException : public BaseException {
	public:
		explicit NotImplementedException(const std::string &msg) : BaseException(msg) {};
	};

	class APIErrorException : public BaseException {
	private:
		int _code;
		std::string _value;

	public:
		explicit APIErrorException(const std::string &msg, const std::string &val, int code) :
			BaseException(msg),
			_code(code), _value(val) {};
		std::string getBody() { return this->_value; };
		int getCode() { return this->_code; };
	};

	class EmptyValueException : public BaseException {
	public:
		explicit EmptyValueException(const std::string &msg) : BaseException(msg) {};
	};
}


#endif //DISCXXORD_EXCEPTION_HPP
