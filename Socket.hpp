//
// Created by Gegel85 on 05/04/2019.
//

#ifndef DISC_ORD_SOCKET_HPP
#define DISC_ORD_SOCKET_HPP


#if defined _WIN32 || defined __WIN32 || defined __WIN32__
#	include <winsock.h>
#else
#	include <sys/socket.h>
typedef SOCKET int;
#endif
#include <string>
#include <map>

namespace DisCXXord {
	class Socket {
	public:
		struct HttpRequestIn {
			std::string				method;
			std::string				host;
			int					portno;
			std::map<std::string, std::string>	header;
			std::string				path;
		};

		struct HttpRequestOut {
			std::map<std::string, std::string>	header;
			int					returnCode;
			std::string				codeName;
			std::string				httpVer;
			std::string				body;
		};

		class Exception : public std::exception {
		private:
			std::string	_msg;
		public:
			explicit Exception(const std::string &msg) : _msg(msg) {};
			const char *what() const noexcept override { return this->_msg.c_str(); };
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

		Socket();
		~Socket();
		bool			isOpen();
		virtual void		connect(const std::string &host, unsigned short portno);
		virtual void		connect(unsigned int ip, unsigned short portno);
		virtual void		disconnect();
		virtual void		send(const std::string &);
		virtual std::string	read(int size);
		virtual std::string	readUntilEOF();
		std::string		generateHttpRequest(const HttpRequestIn &);
		HttpRequestOut		makeHttpRequest(const HttpRequestIn &);
		std::string		makeRawRequest(const std::string &host, unsigned short portno, const std::string &content);
		SOCKET			getSockFd() { return this->_sockfd; };

	protected:
		SOCKET		_sockfd = INVALID_SOCKET;
		bool		_opened = false;

		HttpRequestOut	_parseHttpResponse(const std::string &);
	};
}

#endif //DISC_ORD_SOCKET_HPP
